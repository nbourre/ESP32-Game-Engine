#include "Square.h"

Square::Square(int startX, int startY, int size)
    : x(startX), y(startY), size(size) {}

void Square::update(unsigned long deltaTime) {
    // Maybe add movement later?
}

void Square::draw(Renderer& renderer) {
    renderer.drawRectangle(x, y, size, size);
}
