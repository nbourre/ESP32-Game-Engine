#pragma once
#include <vector>
#include <string>
#include <functional>
#include <utility> 
#include <deque>   
#include "Scene.h" 

// --- FreeRTOS Queue ---
#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
// --- End FreeRTOS Queue ---

// Define the logger type directly to break circular dependency with EDGE.h
using EDGELogger = std::function<void(const char* message)>;

// --- Abstract Input Definitions ---
// These enums create a generic, engine-level representation of input
// that is independent of any specific hardware or library.
enum class EDGE_Button {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    OK,
    CANCEL
};

enum class EDGE_Event {
    PRESS,
    RELEASE,
    CLICK,
    LONG_PRESS
};
// --- End Abstract Input Definitions ---


// Forward declarations
class SceneManager;

class InputManager {
public:
    using DeferredAction = std::function<void()>;

    struct DeferredActionEntry {
        Scene* ownerScene;
        DeferredAction action;
    };

    // ListenerInfo now uses the abstract EDGE types
    struct ListenerInfo {
        EDGE_Button button;
        EDGE_Event eventType;
        Scene* scene; 
        DeferredAction callback; 

        bool operator==(const ListenerInfo& other) const {
            return button == other.button && eventType == other.eventType && scene == other.scene;
        }
    };

    void init();
    void update(unsigned long dt); 
    void processQueuedKeys(); 
    void setLogger(EDGELogger logger) { _logger = logger; }

    void setSceneManager(SceneManager* sm);
    void setKeyQueue(QueueHandle_t queue); 
    
    // Method signatures are now updated to use abstract types
    bool registerButtonListener(EDGE_Button button, EDGE_Event eventType, Scene* scene, DeferredAction callback);
    void unregisterButtonListener(EDGE_Button button, EDGE_Event eventType, Scene* scene);
    void unregisterAllListenersForScene(Scene* scene);
    void processButtonEvent(EDGE_Button button, EDGE_Event eventType);

    void clearDeferredActionsForScene(Scene* scene);

private:
    std::vector<ListenerInfo> listeners; 
    SceneManager* sceneManager = nullptr; 
    QueueHandle_t keyQueue = NULL;        
    EDGELogger _logger;
    
    std::deque<DeferredActionEntry> _deferredActionsQueue; 
    void deferAction(Scene* ownerScene, DeferredAction action); 
};