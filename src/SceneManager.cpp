#include "SceneManager.h" 
#include "Scene.h"
#include "InputManager.h"
#include "Renderer.h"
#include <Arduino.h>         
#include <vector>            
 
SceneManager::SceneManager() : _logger(nullptr) {
    // Constructor is now empty. Dependencies will be injected.
}

SceneManager::~SceneManager() {
    clearStack(); 
}

void SceneManager::setLogger(EDGELogger logger) {
    _logger = logger;
    // When the SceneManager gets its logger, it passes it down to the base Scene class static setter
    Scene::setMasterLogger(logger);
}


// --- NEW GETTER IMPLEMENTATIONS ---
bool SceneManager::isSceneChangePending() const { return _pendingSceneChange; }
String SceneManager::getPendingSceneName() const { return _pendingNextSceneName; }
void* SceneManager::getPendingConfigData() const { return _pendingConfigData; }
bool SceneManager::getPendingReplaceStack() const { return _pendingReplaceStack; }
// --- END NEW GETTER IMPLEMENTATIONS ---

void SceneManager::processSceneChanges() {
    if (!_pendingSceneChange) {
        return;
    }

    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Processing scene change request. Target: %s, Replace: %s", _pendingNextSceneName.c_str(), _pendingReplaceStack ? "true" : "false"); _logger(buf); }

    String nameToSet = _pendingNextSceneName;
    bool replace = _pendingReplaceStack;
    void* configPtr = _pendingConfigData;

    if (nameToSet != "UNKNOWN" && nameToSet != "") {
        if (replace) {
            setCurrentScene(nameToSet, configPtr);
        } else {
            pushScene(nameToSet, configPtr);
        }
    } else {
        if (_logger) _logger("[SCENEMANAGER] Scene change NOT processed. Target name was invalid.");
    }

    clearPendingSceneChange();
}

void SceneManager::setInputManager(InputManager* manager) {
    inputManager = manager;
}


bool SceneManager::registerScene(const String& name, SceneFactoryFunction factory) {
    if (!factory) {
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Tried to register a null factory for Scene '%s'", name.c_str()); _logger(buf); }
        return false;
    }
    if (name.isEmpty()) {
        if (_logger) _logger("[SCENEMANAGER] Scene name cannot be empty for registration.");
        return false;
    }

    bool replacedFactory = _sceneFactories.count(name);

    if (replacedFactory) {
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Overwriting factory registration for Scene '%s'.", name.c_str()); _logger(buf); }
    }
    
    _sceneFactories[name] = factory;

    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Registered Scene '%s' with its factory.", name.c_str()); _logger(buf); }
    return true;
}

void SceneManager::requestSetCurrentScene(const String& sceneName, void* configData) {
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Requesting to SET current scene to '%s'", sceneName.c_str()); _logger(buf); }
    
    _pendingNextSceneName = sceneName;
    _pendingConfigData = configData;
    _pendingReplaceStack = true;
    _pendingSceneChange = true;
}

void SceneManager::requestPushScene(const String& sceneName, void* configData) {
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Requesting to PUSH scene '%s'", sceneName.c_str()); _logger(buf); }

    _pendingNextSceneName = sceneName;
    _pendingConfigData = configData;
    _pendingReplaceStack = false;
    _pendingSceneChange = true;
}

void SceneManager::clearPendingSceneChange() {
    _pendingSceneChange = false;
    _pendingNextSceneName = "";
    if (_pendingConfigData) {
        _pendingConfigData = nullptr;
    }
}

void SceneManager::clearStack() {
    if (_logger) _logger("[SCENEMANAGER] Clearing scene stack.");
    for (int i = sceneCount - 1; i >= 0; --i) {
        if (sceneStack[i]) {
            if (inputManager) {
                inputManager->unregisterAllListenersForScene(sceneStack[i]);
                inputManager->clearDeferredActionsForScene(sceneStack[i]);
            }
            if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Deleting scene '%s' (%p) from stack index %d", _sceneNameStack[i].c_str(), sceneStack[i], i); _logger(buf); }
            delete sceneStack[i];
            sceneStack[i] = nullptr;
            _sceneNameStack[i] = "";
        }
    }
    sceneCount = 0;
}

Scene* SceneManager::createSceneByName(const String& sceneName, void* configData) {
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Attempting to create scene '%s' using factory.", sceneName.c_str()); _logger(buf); }
    
    auto it = _sceneFactories.find(sceneName);
    if (it == _sceneFactories.end()) {
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] No factory registered for Scene '%s'!", sceneName.c_str()); _logger(buf); }
        return nullptr;
    }

    SceneFactoryFunction& factory = it->second;
    Scene* newScene = factory(configData); 

    if (newScene == nullptr) {
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Factory for Scene '%s' returned null!", sceneName.c_str()); _logger(buf); }
        return nullptr;
    }

    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Factory created scene '%s' at %p, calling generic init...", sceneName.c_str(), newScene); _logger(buf); }
    newScene->init(); 

    return newScene;
}


bool SceneManager::setCurrentScene(const String& sceneName, void* configData) { 
    if (!inputManager) { 
        if (_logger) _logger("[SCENEMANAGER] InputManager is null in setCurrentScene.");
        return false; 
    }
    if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Setting current scene to '%s'", sceneName.c_str()); _logger(buf); }

    clearStack(); 

    Scene* newScene = createSceneByName(sceneName, configData); 
    if (!newScene) { 
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Failed to create scene '%s' for setCurrentScene.", sceneName.c_str()); _logger(buf); }
        return false; 
    }

    sceneStack[sceneCount] = newScene;
    _sceneNameStack[sceneCount] = sceneName;
    sceneCount++;
    newScene->onEnter();

    return true;
}

bool SceneManager::pushScene(const String& sceneName, void* configData) { 
     if (!inputManager) { 
        if (_logger) _logger("[SCENEMANAGER] InputManager is null in pushScene.");
        return false; 
     }
     if (sceneCount >= MAX_SCENES) { 
        if (_logger) _logger("[SCENEMANAGER] Scene stack full, cannot push.");
        return false; 
     }
     if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Pushing scene '%s'", sceneName.c_str()); _logger(buf); }

    if (sceneCount > 0 && sceneStack[sceneCount - 1]) {
        sceneStack[sceneCount - 1]->onExit();
    }

    Scene* newScene = createSceneByName(sceneName, configData); 
     if (!newScene) { 
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Failed to create scene '%s' for pushScene.", sceneName.c_str()); _logger(buf); }
        if (sceneCount > 0 && sceneStack[sceneCount - 1]) {
            sceneStack[sceneCount - 1]->onEnter(); 
        }
        return false;
    }

    sceneStack[sceneCount] = newScene;
    _sceneNameStack[sceneCount] = sceneName;
    sceneCount++;
    newScene->onEnter();

    return true;
}

bool SceneManager::popScene() {
     if (!inputManager) { 
        if (_logger) _logger("[SCENEMANAGER] InputManager is null in popScene.");
        return false; 
     }
    if (sceneCount > 0) {
        Scene* removedScene = sceneStack[sceneCount - 1];
        String removedSceneName = _sceneNameStack[sceneCount - 1];
        if (_logger) { char buf[128]; snprintf(buf, sizeof(buf), "[SCENEMANAGER] Popping scene '%s'", removedSceneName.c_str()); _logger(buf); }

        if (removedScene) {
            removedScene->onExit();
            inputManager->unregisterAllListenersForScene(removedScene);
            inputManager->clearDeferredActionsForScene(removedScene);
            delete removedScene;
        }
        sceneStack[sceneCount - 1] = nullptr;
        _sceneNameStack[sceneCount -1] = "";
        sceneCount--;

        if (sceneCount > 0 && sceneStack[sceneCount - 1]) {
             sceneStack[sceneCount - 1]->onEnter();
        }
        return true;
    } else { 
        if (_logger) _logger("[SCENEMANAGER] Attempted to pop from an empty scene stack.");
        return false; 
    }
}

void SceneManager::update(unsigned long dt) {
    if (sceneCount > 0 && sceneStack[sceneCount - 1]) {
        sceneStack[sceneCount - 1]->update(dt);
    }
}

void SceneManager::draw(Renderer& rendererRef) { 
    if (sceneCount > 0 && sceneStack[sceneCount - 1]) {
        sceneStack[sceneCount - 1]->draw(rendererRef);
    }
}

Scene* SceneManager::getCurrentScene() const {
    if (sceneCount > 0) {
        return sceneStack[sceneCount - 1];
    }
    return nullptr;
}

String SceneManager::getCurrentSceneName() const {
    if (sceneCount > 0) {
        return _sceneNameStack[sceneCount - 1];
    }
    return "";
}

SceneFactoryFunction SceneManager::getFactoryByName(const String& name) const {
    auto it = _sceneFactories.find(name);
    if (it != _sceneFactories.end()) {
        return it->second;
    }
    return nullptr;
}

std::vector<String> SceneManager::getRegisteredSceneNames() const {
    std::vector<String> names;
    for (const auto& pair : _sceneFactories) {
        names.push_back(pair.first);
    }
    return names;
}