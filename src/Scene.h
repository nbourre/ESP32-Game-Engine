#pragma once
#include <ArduinoQueue.h>
#include <functional>

// Define the logger type here, as it's a fundamental type the scene can use.
using EDGELogger = std::function<void(const char* message)>;

#define MAX_ENTITIES 10  

// --- Forward declarations ---
class Renderer;
class InputManager; 

class Entity;

enum class SceneType {
    UNKNOWN, 
    BOOT,
    MAIN,
    STATS,
    PARAMS,
    ACTION,
    SLEEPING,
    PLAY_MENU,
    FLAPPY_GAME,
    LANGUAGE_SELECT_PREQUEL,
    PREQUEL_STAGE_1,
    PREQUEL_STAGE_2, 
    PREQUEL_STAGE_3,
    PREQUEL_STAGE_4          
}; 

class Scene {
public:
    virtual ~Scene(); 

    virtual void init(); 
    virtual void onEnter() {}; 
    virtual void onExit() {};  
    virtual void update(unsigned long deltaTime);
    virtual void draw(Renderer& renderer); 

    bool doesManageOwnDrawing() const { return managesOwnDrawing; }
    virtual bool usesKeyQueue() const { return false; }
    virtual void processKeyPress(uint8_t keyCode) {}
    virtual SceneType getSceneType() const { return SceneType::UNKNOWN; }
    
    virtual class DialogBox* getDialogBox() { return nullptr; }

    static void setMasterLogger(EDGELogger logger) { _masterLogger = logger; }

protected:
    ArduinoQueue<Entity*> entities;
    
    bool managesOwnDrawing = false;
    
    // Static logger for all scene instances to use
    static EDGELogger _masterLogger;
};