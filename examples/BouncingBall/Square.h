#pragma once
#include "Entity.h"

class Square : public Entity {
public:
    Square(int startX, int startY, int size);
    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;

private:
    float x, y;
    int size;
};
