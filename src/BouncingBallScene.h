#pragma once
#include "Scene.h"
#include "Renderer.h"

class BouncingBallScene : public Scene {
public:
    void init() override;
    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;
    
};
