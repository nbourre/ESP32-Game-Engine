#pragma once
#include "SceneManager.h"
#include "Renderer.h"
#include "InputManager.h"

class EDGE {
public:
    EDGE();
    ~EDGE();

    void init();
    void update();
    void draw();

    unsigned long getDeltaTime() const { return deltaTime; }
    
    // Scene management via SceneManager
    void setScene(Scene* newScene);
    
    void setRenderer(Renderer& newRenderer) { renderer = newRenderer; }
    Renderer& getRenderer();
    InputManager& getInputManager() { return inputManager; }

private:
    SceneManager sceneManager;
    Renderer renderer;
    InputManager inputManager;
    unsigned long previousMillis;
    unsigned long deltaTime;
};
