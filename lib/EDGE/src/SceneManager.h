#pragma once
#include "Scene.h"
#include "Renderer.h"

class SceneManager {
public:
    SceneManager();
    void setCurrentScene(Scene* newScene);
    void update(unsigned long dt);
    void draw(Renderer& renderer);
    Scene* getCurrentScene() const;

private:
    Scene* currentScene;
};
