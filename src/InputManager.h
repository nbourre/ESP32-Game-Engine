#pragma once
#include <Arduino.h>
#include "InputConfig.h"

class InputManager {
public:
    InputManager(const InputConfig& config);
    void init();
    void update(unsigned long dt);
    bool isButtonPressed(uint8_t buttonIndex);   // True on press for specific button
    bool isButtonReleased(uint8_t buttonIndex);  // True on release for specific button
    bool isButtonClicked(uint8_t buttonIndex);   // True on press and release for specific button
    bool isButtonDown(uint8_t buttonIndex);      // True while button is pressed for specific button

private:
    InputConfig config;

    bool* buttonState;      // States of buttons (pressed or not)
    bool* stateChanged;     // Track state change for each button
    uint16_t* waitTime;     // Debounce time for each button
    bool* clickFlag;        // Used for isButtonClicked function for each button
    uint8_t* buttonPins;    // Array to store button pins
};
