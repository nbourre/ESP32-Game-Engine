#include "BouncingBallScene.h"
#include "Renderer.h"  // Include Renderer for drawing functions
#include "EDGE.h"      // Include EDGE to access the renderer instance
#include "Ball.h"      // Include Ball entity


extern EDGE engine;  // Get access to the global game engine

void BouncingBallScene::init() {
    addEntity(new Ball(36, 20, 15));

}

void BouncingBallScene::update(unsigned long deltaTime) {
    Scene::update(deltaTime);
}

void BouncingBallScene::draw(Renderer& renderer) {
    Scene::draw(renderer);

    

    createSafeString(fpsText, 32);
    fpsText.printf("FPS:%d", 1000 / engine.getDeltaTime());
    renderer.drawTextSafe(10, 10, fpsText.c_str());
   
}