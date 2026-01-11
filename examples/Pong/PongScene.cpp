#include "PongScene.h"
#include "EDGE.h"
extern EDGE engine;

#define PADDLE_WIDTH 10
#define PADDLE_HEIGHT 50
#define BALL_RADIUS 6
#define BALL_SPEED 120.0f
#define SCORE_TO_WIN 5

void PongScene::init() {
    int screenWidth = engine.getRenderer().getWidth();
    int screenHeight = engine.getRenderer().getHeight();

    leftPaddle = new PaddleEntity(0, screenHeight/2 - PADDLE_HEIGHT/2, PADDLE_WIDTH, PADDLE_HEIGHT, false);
    rightPaddle = new PaddleEntity(screenWidth - PADDLE_WIDTH, screenHeight/2 - PADDLE_HEIGHT/2, PADDLE_WIDTH, PADDLE_HEIGHT, true);
    ball = new BallEntity(screenWidth/2, screenHeight/2, BALL_RADIUS, BALL_SPEED);
    ball->reset();

    addEntity(leftPaddle);
    addEntity(rightPaddle);
    addEntity(ball);

    leftScore = 0;
    rightScore = 0;
    gameOver = false;
}

void PongScene::update(unsigned long deltaTime) {
    if (!gameOver) {
        // --- Input Player ---
        leftPaddle->velocity = 0;
        if (engine.getInputManager().isButtonDown(0)) leftPaddle->velocity = -100.0f;
        if (engine.getInputManager().isButtonDown(1)) leftPaddle->velocity = 100.0f;

        // --- Update actors ---
        leftPaddle->update(deltaTime);
        rightPaddle->update(deltaTime);
        ball->update(deltaTime);

        // --- Collisions between entities ---
        // Collision handling is performed by the base Scene::update

        // --- Check if ball is out of bounds ---
        if (ball->x < 0) {       // left side
            rightScore++;
            ball->reset();
        }
        if (ball->x > engine.getRenderer().getWidth()) { // right side
            leftScore++;
            ball->reset();
        }

        // --- Game Over ---
        if (leftScore >= SCORE_TO_WIN || rightScore >= SCORE_TO_WIN) {
            gameOver = true;
        }

    } else {
        if (engine.getInputManager().isButtonPressed(0)) resetGame();
    }
}

void PongScene::draw(Renderer& renderer) {
    // draw scores
    renderer.drawTextSafe(20, 8, "%d", leftScore);
    renderer.drawTextSafe(engine.getRenderer().getWidth() - 30, 8, "%d", rightScore);

    if (gameOver) {
        // draw game over message
        renderer.drawTextSafe(engine.getRenderer().getWidth()/2 - 40,
                              engine.getRenderer().getHeight()/2,
                              "GAME OVER!");
    }

    // draw entities (paddles and ball)
    Scene::draw(renderer);
}

void PongScene::resetGame() {
    leftScore = 0;
    rightScore = 0;
    gameOver = false;
    ball->reset();
}
