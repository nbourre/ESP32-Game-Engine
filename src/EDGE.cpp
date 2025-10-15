#include "EDGE.h"
#include "InputConfig.h"

EDGE::EDGE(const DisplayConfig& displayConfig, const InputConfig& inputConfig) : renderer(displayConfig), inputManager(inputConfig) {
    previousMillis = 0;
    deltaTime = 0;
}

EDGE::EDGE(const DisplayConfig& displayConfig) : renderer(displayConfig), inputManager(InputConfig(0)) {
    previousMillis = 0;
    deltaTime = 0;
}

EDGE::~EDGE() {}

void EDGE::init() {
    renderer.init();
    inputManager.init();
}

void EDGE::update() {
    unsigned long currentMillis = millis();
    deltaTime = currentMillis - previousMillis;
    previousMillis = currentMillis;

    inputManager.update(deltaTime);
    sceneManager.update(deltaTime);
}

void EDGE::draw() {
    renderer.beginFrame();
    sceneManager.draw(renderer);
    renderer.endFrame();
}

void EDGE::setScene(Scene* newScene) {
    sceneManager.setCurrentScene(newScene);
}

Renderer& EDGE::getRenderer() {
    return renderer;
}