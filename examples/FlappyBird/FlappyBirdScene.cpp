#include "FlappyBirdScene.h"
#include "EDGE.h"

extern EDGE engine;

#define GRAVITY 0.15f
#define JUMP_FORCE -2.5f
#define PIPE_SPEED 1
#define PIPE_WIDTH 10
#define PIPE_GAP 15
#define TOP_BOTTOM_PADDING 10

void FlappyBirdScene::init() {
    screenHeight = engine.getRenderer().getHeight();
    screenWidth = engine.getRenderer().getWidth();

    birdY = screenHeight / 2;
    birdX = 10;
    birdVelocity = 0;
    pipeX = screenWidth;
    pipeGapY = random(10, screenHeight - PIPE_GAP - 10);
    gameOver = false;
    score = 0;
}

void FlappyBirdScene::update(unsigned long deltaTime) {
    if (!gameOver) {
        // Handle input (button press to jump)
        if (engine.getInputManager().isButtonPressed(0)) {
            birdVelocity = JUMP_FORCE;
        }

        // Apply gravity
        birdVelocity += GRAVITY;
        birdY += birdVelocity;

        // Move pipes left
        pipeX -= PIPE_SPEED;

        if (pipeX == 8) {  // Bird passes the pipe
            score++;
            birdX += 2;  // Move bird forward
        }

        // If pipe goes off screen, reset it
        if (pipeX < -PIPE_WIDTH) {
            pipeX = screenWidth;
            pipeGapY = random(10, screenHeight - PIPE_GAP - 10);
        }

        bool outOfBounds = birdY < -TOP_BOTTOM_PADDING || birdY > screenHeight + TOP_BOTTOM_PADDING;

        // Check collision
        if (outOfBounds || 
            (pipeX < birdX + 2 && pipeX + PIPE_WIDTH > birdX - 2 &&  // Check if bird is within pipe's X range
            (birdY < pipeGapY || birdY > pipeGapY + PIPE_GAP))) {
            gameOver = true;
        }

    } else {
        // Restart game on button press
        if (engine.getInputManager().isButtonPressed(0)) {
            resetGame();
        }
    }
}

void FlappyBirdScene::draw(Renderer& renderer) {
    renderer.drawCircle(10, birdY, 2);  // Bird
    renderer.drawFilledRectangle(pipeX, 0, PIPE_WIDTH, pipeGapY);  // Top Pipe
    renderer.drawFilledRectangle(pipeX, pipeGapY + PIPE_GAP, PIPE_WIDTH, screenHeight - (pipeGapY + PIPE_GAP));  // Bottom Pipe

    renderer.setFont(u8g2_font_6x10_tf);
    renderer.drawTextSafe(screenWidth - 15, 8, "%d", score);


    if (gameOver) {
        renderer.setFont(u8g2_font_6x10_tf);
        renderer.drawTextSafe(15, screenHeight / 2, "Game Over!");
    }
}

void FlappyBirdScene::resetGame() {
    init();  // Reset variables
}
