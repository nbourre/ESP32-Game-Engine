#include "BouncingBallScene.h"
#include "Renderer.h"  // Include Renderer for drawing functions
#include "EDGE.h"      // Include EDGE to access the renderer instance

extern EDGE engine;  // Get access to the global game engine

void BouncingBallScene::init() {
    ballX = 36;  // Start at screen center (assuming 72x40 display)
    ballY = 20;
    speedX = 1.5;
    speedY = 1.2;
    ballRadius = 2;
}

void BouncingBallScene::update(unsigned long deltaTime) {
    // Move the ball
    ballX += speedX;
    ballY += speedY;

    // Bounce off edges
    if (ballX - ballRadius <= 0 || ballX + ballRadius >= 72) {
        speedX = -speedX;
    }
    if (ballY - ballRadius <= 0 || ballY + ballRadius >= 40) {
        speedY = -speedY;
    }
}

void BouncingBallScene::draw() {
    engine.getRenderer().drawCircle(ballX, ballY, ballRadius);  // Use EDGE's renderer
}
