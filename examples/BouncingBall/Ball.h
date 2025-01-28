#pragma once
#include "Entity.h"

class Ball : public Entity {
public:
    Ball(int startX, int startY, int radius);
    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;

private:
    float x, y;
    float speedX, speedY;
    int radius;
};
