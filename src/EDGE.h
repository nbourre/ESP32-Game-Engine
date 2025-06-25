#pragma once

#include <functional> // For std::function

// --- Engine-level type definitions ---
// Define the logger callback type here to be accessible by other engine headers.
using EDGELogger = std::function<void(const char* message)>;

// --- Full definitions for member variables ---
// To resolve "incomplete type" errors, the compiler needs the full class
// definition for any member variable that is stored by value.
#include "SceneManager.h"
#include "InputManager.h"
#include "Renderer.h"
#include "DisplayConfig.h"


// --- Forward declarations ---
class Scene; 

class EDGE {
public:
    EDGE(U8G2* u8g2_ptr, const DisplayConfig& displayConf, EDGELogger logger = nullptr);
    ~EDGE();

    void init();
    void update();
    void draw();

    unsigned long getDeltaTime() const { return deltaTime; }

    Renderer& getRenderer();
    InputManager& getInputManager();
    SceneManager& getSceneManager();
    
private:
    EDGELogger _logger; // Logger callback provided by the application

    SceneManager sceneManager;
    Renderer     renderer;
    InputManager inputManager;

    unsigned long previousMillis;
    unsigned long deltaTime;
};