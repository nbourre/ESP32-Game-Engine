#pragma once
#include "Entity.h"

class PaddleEntity : public Entity  {
public:
    float x, y;
    int width, height;
    float velocity;      // for player movement
    float accumulator;   // for AI movement
    bool isAI;

    PaddleEntity(float x, float y, int w, int h, bool ai = false)
        : x(x), y(y), width(w), height(h), velocity(0), accumulator(0), isAI(ai) {}

    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;

    Rect getHitBox() override { return {x, y, width, height}; }

    void onCollision(Entity* other) override;
};
