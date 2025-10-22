#pragma once
#include "SceneManager.h"
#include "Renderer.h"
#include "InputManager.h"
#include "InputConfig.h"
#include "DisplayConfig.h"
//these two dont need anything to happen to them
//they are here so you can reference them from anywhere
//they just need to be refrenced by the engine to be "initialized"
#include "FontInit.h"
#include "BitmapInit.h"



class EDGE {
public:
    EDGE(const DisplayConfig& displayConfig, const InputConfig& inputConfig);
    EDGE(const DisplayConfig& displayConfig);
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
