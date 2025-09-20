#include "Arduino.h"
#include "EDGE.h"




#include "BouncingBallScene.h"

#define SCK_PIN    18  // Custom SCK (clock) pin
#define MOSI_PIN   19 // Custom MOSI (data out) pin
#define CS_PIN     25  // Chip select pin
#define DC_PIN     27  // Data/Command pin
#define RST_PIN    26  // Reset pin

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Examples ESP32-C3 with OLED Display
DisplayConfig config(SSD1309, U8G2_R0, SCK_PIN, MOSI_PIN, CS_PIN, DC_PIN, RST_PIN, SCREEN_WIDTH, SCREEN_HEIGHT, false);

EDGE engine(config);
BouncingBallScene bouncingScene;

unsigned long currentTime = 0;

void printTask(ulong time);

void setup() {
    Serial.begin(115200);
    delay(100);

    engine.init();
    engine.setScene(&bouncingScene);
        
    Serial.println("Setup completed");
}

void loop() {
    currentTime = millis();
    engine.update();
    engine.draw();
    
    printTask(currentTime);
}

void printTask(unsigned long ct) {
  // If serial output is not working set "USB CDC on BOOT" to "Enabled" in tools
  static unsigned long lastTime = 0;
  const int rate = 2000;
  
  if (ct - lastTime < rate) return;
  
  lastTime = ct;
  
  Serial.print(".");
}
