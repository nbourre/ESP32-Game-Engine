#include "Ball.h"
#include "EDGE.h"

Ball::Ball(int startX, int startY, int radius)
    : x(startX), y(startY), speedX(1.5), speedY(1.2), radius(radius) {}

void Ball::update(unsigned long deltaTime) {
    x += speedX;
    y += speedY;

    if (x + 15 <= 0 || x + 15 >= 128) {
        speedX = -speedX;
    }
    if (y - 7 <= 0 || y + 7 >= 64) {
        speedY = -speedY;
    }
}

void Ball::draw(Renderer& renderer) {
    //replace ball with EDGE logo
    renderer.drawXBM(x, y, 15, 7, EDGE_Icon_bits);
}
