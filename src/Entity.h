#pragma once
#include "Renderer.h"

struct Rect {
    float x, y;
    int width, height;

    bool intersects(const Rect& other) const {
        return !(x + width < other.x || x > other.x + other.width ||
                 y + height < other.y || y > other.y + other.height);
    }
};

class Entity {
public:
    virtual ~Entity() {}
    virtual void update(unsigned long deltaTimet) = 0;
    virtual void draw(Renderer& renderer) = 0;

    virtual Rect getHitBox() = 0;
    virtual void onCollision(Entity* other) = 0;
};
