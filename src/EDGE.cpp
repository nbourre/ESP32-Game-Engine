#include "EDGE.h"
#include "Scene.h" 
#include <Arduino.h>

// Constructor - Updated to accept a logger callback
EDGE::EDGE(U8G2* u8g2_ptr, const DisplayConfig& displayConf, EDGELogger logger)
    : _logger(logger),
      sceneManager(), 
      renderer(u8g2_ptr, displayConf),
      inputManager(),
      previousMillis(0),
      deltaTime(0)
{
    if (_logger) {
        _logger("EDGE Engine core constructed.");
    }
}

// Destructor
EDGE::~EDGE() {}

void EDGE::init() {
    // Propagate the logger to all engine sub-components
    renderer.setLogger(_logger);
    inputManager.setLogger(_logger);
    sceneManager.setLogger(_logger);
    
    renderer.init();
    inputManager.init();
    sceneManager.setInputManager(&inputManager);
    inputManager.setSceneManager(&sceneManager); 

    if (_logger) {
        _logger("EDGE Engine initialized.");
    }
}

void EDGE::update() {
    unsigned long currentMillis = millis();
    deltaTime = currentMillis - previousMillis;
    previousMillis = currentMillis;

    sceneManager.processSceneChanges();
    inputManager.update(deltaTime); 
    sceneManager.update(deltaTime); 
}

void EDGE::draw() {
    Scene* currentScene = sceneManager.getCurrentScene();
    U8G2* u8g2_ptr = renderer.getU8G2();
    if (!u8g2_ptr) {
        return;
    }

    if (currentScene && currentScene->doesManageOwnDrawing()) {
        sceneManager.draw(renderer); 
    } else {
        u8g2_ptr->firstPage();
        do {
            sceneManager.draw(renderer); 
        } while (u8g2_ptr->nextPage());
    }
}

Renderer& EDGE::getRenderer() { return renderer; }
InputManager& EDGE::getInputManager() { return inputManager; }
SceneManager& EDGE::getSceneManager() { return sceneManager; }