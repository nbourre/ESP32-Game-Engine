#include "SceneManager.h"

SceneManager::SceneManager() : currentScene(nullptr) {}

void SceneManager::setCurrentScene(Scene* newScene) {
    if (currentScene) {
        currentScene->exit();
    }
    currentScene = newScene;
    if (currentScene) {
        currentScene->init();
    }
}

void SceneManager::update(unsigned long dt) {
    if (currentScene) {
        currentScene->update(dt);
    }
}

void SceneManager::draw(Renderer& renderer) {
    if (currentScene) {
        currentScene->draw();
    }
}

Scene* SceneManager::getCurrentScene() const {
    return currentScene;
}
