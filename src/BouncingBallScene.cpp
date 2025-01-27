#include "BouncingBallScene.h"
#include "Renderer.h"  // Include Renderer for drawing functions
#include "EDGE.h"      // Include EDGE to access the renderer instance
#include "Ball.h"      // Include Ball entity
#include "Square.h"    // Include Square entity

extern EDGE engine;  // Get access to the global game engine

void BouncingBallScene::init() {
    addEntity(new Ball(36, 20, 5));
    addEntity(new Square(10, 10, 8));
}

void BouncingBallScene::update(unsigned long deltaTime) {
    Scene::update(deltaTime);
}

void BouncingBallScene::draw(Renderer& renderer) {
    Scene::draw(renderer);

    // ✅ Safe formatted text example (FPS counter)
    createSafeString(fpsText, 32);
    fpsText.printf("FPS: %d", 1000 / engine.getDeltaTime());
    renderer.drawTextSafe(5, 10, fpsText.c_str());

    // ✅ Safe dynamic string rendering
    createSafeString(scoreText, 32);
    scoreText = "Score: 100";
    renderer.drawTextSafe(5, 25, scoreText.c_str());
}