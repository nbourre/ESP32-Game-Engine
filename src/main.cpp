#include <Arduino.h>
#include "EDGE.h"
#include "BouncingBallScene.h"

EDGE engine;
BouncingBallScene bouncingScene;

unsigned long currentTime = 0;

void printTask(ulong time);

void setup() {
    Serial.begin(115200);
    engine.init();
    engine.setScene(&bouncingScene);

    Serial.println("Setup completed");
}

void loop() {
    currentTime = millis();
    engine.update();
    engine.draw();
}
