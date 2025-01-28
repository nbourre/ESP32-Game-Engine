#pragma once
#include "Renderer.h"

class Entity {
public:
    virtual ~Entity() {}
    virtual void update(unsigned long deltaTime) = 0;
    virtual void draw(Renderer& renderer) = 0;
};
