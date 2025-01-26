#pragma once
#include "Scene.h"
#include "Renderer.h"

class BouncingBallScene : public Scene {
public:
    void init() override;
    void update(unsigned long deltaTime) override;
    void draw() override;

private:
    float ballX, ballY;
    float speedX, speedY;
    int ballRadius;
};
