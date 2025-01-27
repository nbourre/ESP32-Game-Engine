#include "Ball.h"

Ball::Ball(int startX, int startY, int radius)
    : x(startX), y(startY), speedX(1.5), speedY(1.2), radius(radius) {}

void Ball::update(unsigned long deltaTime) {
    x += speedX;
    y += speedY;

    if (x - radius <= 0 || x + radius >= 72) {
        speedX = -speedX;
    }
    if (y - radius <= 0 || y + radius >= 40) {
        speedY = -speedY;
    }
}

void Ball::draw(Renderer& renderer) {
    renderer.drawCircle(x, y, radius);
}
