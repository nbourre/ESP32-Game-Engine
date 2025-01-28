#pragma once
#include "Scene.h"
#include "Entity.h"

class FlappyBirdScene : public Scene {
public:
    void init() override;
    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;

private:
    float birdY, birdVelocity;
    float birdX;
    bool gameOver;
    int pipeX, pipeGapY;
    int score;
    
    void resetGame();
};
