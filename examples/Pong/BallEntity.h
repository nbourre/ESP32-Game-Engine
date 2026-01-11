#pragma once
#include "Entity.h"

class BallEntity : public Entity  {
public:
    float x, y, vx, vy;
    float speed;
    int radius;
    unsigned long respawnTimer;   // respawn delay timer
    bool active;                  // is active

    BallEntity(float x, float y, int radius, float speed)
        : x(x), y(y), vx(0), vy(0), radius(radius), speed(speed), respawnTimer(0), active(false) {}
    void reset();

    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;

    Rect getHitBox() override { return {x-radius, y-radius, radius*2, radius*2}; }

    void onCollision(Entity* other);
};
