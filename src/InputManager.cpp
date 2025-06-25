#include "InputManager.h"
#include "SceneManager.h" 
#include "GEM_u8g2.h"     
#include <algorithm>      
#include <Arduino.h>      
#include <deque> 

extern void updateLastActivityTime();


void InputManager::init() {
    listeners.clear();
    _deferredActionsQueue.clear(); 
    keyQueue = NULL; 
}

void InputManager::update(unsigned long dt) {
    int actionsToProcessThisFrame = _deferredActionsQueue.size(); 
    actionsToProcessThisFrame = std::min(actionsToProcessThisFrame, 3); 
    
    for (int i = 0; i < actionsToProcessThisFrame && !_deferredActionsQueue.empty(); ++i) {
        DeferredActionEntry entry = _deferredActionsQueue.front();
        _deferredActionsQueue.pop_front(); 

        if (entry.action) { 
            if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] Executing deferred action for scene %p.", entry.ownerScene); _logger(buf); }
            entry.action(); 
        }
    }
}

void InputManager::processQueuedKeys() {
    if (keyQueue == NULL) return;

    uint8_t receivedKeyCode;
    if (xQueueReceive(keyQueue, &receivedKeyCode, 0) == pdPASS) {
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Received GEM Key %d from queue", receivedKeyCode); _logger(buf); }

        if (!sceneManager) {
            if (_logger) _logger("[HARDWARE_INPUT] InputManager Error: SceneManager is NULL in processQueuedKeys.");
            return;
        }

        Scene* currentScene = sceneManager->getCurrentScene();
        if (!currentScene) {
            if (_logger) _logger("[HARDWARE_INPUT] InputManager Warning: No active scene to process queued key.");
            return;
        }
        
        if (currentScene->usesKeyQueue()) {
            if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENES] InputManager: Forwarding key %d to scene %p (type %d) for processing", receivedKeyCode, (void*)currentScene, (int)currentScene->getSceneType()); _logger(buf); }
            currentScene->processKeyPress(receivedKeyCode);
        } else {
            if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENES] InputManager: Key %d received, but scene %p (type %d) does not use key queue.", receivedKeyCode, (void*)currentScene, (int)currentScene->getSceneType()); _logger(buf); }
        }
    }
}


void InputManager::setSceneManager(SceneManager* sm) {
    sceneManager = sm;
}

void InputManager::setKeyQueue(QueueHandle_t queue) {
    keyQueue = queue;
}

bool InputManager::registerButtonListener(EDGE_Button button, EDGE_Event eventType, Scene* scene, DeferredAction callback) {
    if (!scene || !callback) {
        if (_logger) _logger("[HARDWARE_INPUT] Error: Invalid scene or callback provided for listener registration.");
        return false;
    }
    listeners.push_back({button, eventType, scene, callback}); 
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Registered listener for button %d, event %d, scene %p", (int)button, (int)eventType, scene); _logger(buf); }
    return true;
}

void InputManager::unregisterButtonListener(EDGE_Button button, EDGE_Event eventType, Scene* scene) {
     listeners.erase(
        std::remove_if(listeners.begin(), listeners.end(),
                       [button, eventType, scene](const ListenerInfo& listener) {
                           return listener.button == button && listener.eventType == eventType && listener.scene == scene;
                       }),
        listeners.end());
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Unregistered listener for button %d, event %d, scene %p", (int)button, (int)eventType, scene); _logger(buf); }
}

void InputManager::unregisterAllListenersForScene(Scene* scene) {
    if (!scene) return;
    listeners.erase(
        std::remove_if(listeners.begin(), listeners.end(),
                       [scene](const ListenerInfo& listener) {
                           return listener.scene == scene;
                       }),
        listeners.end());
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Unregistered all listeners for scene %p", scene); _logger(buf); }
}

void InputManager::processButtonEvent(EDGE_Button button, EDGE_Event eventType) {
    updateLastActivityTime(); 

    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager::processButtonEvent: Received button %d, event %d", (int)button, (int)eventType); _logger(buf); }

    if (!sceneManager) {
        if (_logger) _logger("[HARDWARE_INPUT] InputManager Warning: SceneManager not set during event processing!");
        return;
    }

    Scene* currentScene = sceneManager->getCurrentScene();
    if (!currentScene) {
        if (_logger) _logger("[HARDWARE_INPUT] InputManager Warning: No active scene to process event!");
        return; 
    }
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Current scene type: %d, usesKeyQueue: %s", (int)currentScene->getSceneType(), currentScene->usesKeyQueue() ? "true" : "false"); _logger(buf); }

    bool useKeyQueue = currentScene->usesKeyQueue();

    if (useKeyQueue && keyQueue != NULL) {
        uint8_t gemKeyCode = GEM_KEY_NONE;
        if (button == EDGE_Button::UP && eventType == EDGE_Event::CLICK) { gemKeyCode = GEM_KEY_UP; } 
        else if (button == EDGE_Button::DOWN && eventType == EDGE_Event::CLICK) { gemKeyCode = GEM_KEY_DOWN; } 
        else if (button == EDGE_Button::OK && eventType == EDGE_Event::CLICK) { gemKeyCode = GEM_KEY_OK; } 
        else if (button == EDGE_Button::OK && eventType == EDGE_Event::LONG_PRESS) { gemKeyCode = GEM_KEY_CANCEL; }
        else if (button == EDGE_Button::CANCEL && eventType == EDGE_Event::CLICK) { gemKeyCode = GEM_KEY_CANCEL; }

        if (gemKeyCode != GEM_KEY_NONE) {
            if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Attempting to send GEM Key %d to queue.", gemKeyCode); _logger(buf); }
            if (xQueueSend(keyQueue, &gemKeyCode, 0) != pdPASS) {
                if (_logger) _logger("[HARDWARE_INPUT] InputManager Warning: Key queue full!");
            } else {
                if (_logger) _logger("[HARDWARE_INPUT] InputManager: GEM Key sent successfully.");
            }
        }
    } else if (useKeyQueue && keyQueue == NULL) {
        if (_logger) _logger("[HARDWARE_INPUT] InputManager Error: Scene uses key queue, but keyQueue is NULL!");
    }

    bool eventDeferred = false;
    for (const auto& listener : listeners) {
        if (listener.button == button && listener.eventType == eventType && listener.scene == currentScene) {
            if (listener.callback) {
                deferAction(currentScene, listener.callback);
                eventDeferred = true;
                if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Deferred direct callback for button %d, event %d on scene %p.", (int)button, (int)eventType, currentScene); _logger(buf); }
            }
        }
    }

    if (!useKeyQueue && !eventDeferred) {
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: No direct callback found or deferred for button %d, event %d on non-queued scene %p.", (int)button, (int)eventType, currentScene); _logger(buf); }
    }
}

void InputManager::deferAction(Scene* ownerScene, DeferredAction action) {
    if (!ownerScene || !action) return;
    _deferredActionsQueue.push_back({ownerScene, action});
}

void InputManager::clearDeferredActionsForScene(Scene* scene) {
    if (!scene) return;
    size_t initialSize = _deferredActionsQueue.size();
    _deferredActionsQueue.erase(
        std::remove_if(_deferredActionsQueue.begin(), _deferredActionsQueue.end(),
                       [scene](const DeferredActionEntry& entry) {
                           return entry.ownerScene == scene;
                       }),
        _deferredActionsQueue.end());
    size_t removedCount = initialSize - _deferredActionsQueue.size();
    if (removedCount > 0) {
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[HARDWARE_INPUT] InputManager: Cleared %u deferred actions for scene %p", (unsigned int)removedCount, scene); _logger(buf); }
    }
}