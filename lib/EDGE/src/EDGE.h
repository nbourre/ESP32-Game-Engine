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
    
    // Scene management via SceneManager
    void setScene(Scene* newScene);
    Renderer& getRenderer();

private:
    SceneManager sceneManager;
    Renderer renderer;
    InputManager inputManager;
    unsigned long previousMillis;
};
