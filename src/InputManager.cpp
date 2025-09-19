#include "InputManager.h"

void InputManager::init() {
    // Initialize button pins
    buttonPins[0] = BUTTON_PIN_1;
    buttonPins[1] = BUTTON_PIN_2;
    buttonPins[2] = BUTTON_PIN_3;
    buttonPins[3] = BUTTON_PIN_4;
    buttonPins[4] = BUTTON_PIN_5;
    
    // Set all pins as INPUT_PULLUP
    for (int i = 0; i < BUTTON_COUNT; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
        buttonState[i] = false;
        stateChanged[i] = false;
        waitTime[i] = 0;
        clickFlag[i] = false;
    }
}

void InputManager::update(unsigned long dt) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        // Check if in debounce time window for each button
        if (waitTime[i] >= dt) {
            waitTime[i] -= dt;
            continue;
        }

        waitTime[i] = 0;
        bool reading = digitalRead(buttonPins[i]) == LOW;
        
        if (reading != buttonState[i]) {
            // Button pressed or released.
            waitTime[i] = 100;  // Debounce delay
            buttonState[i] = reading;
            stateChanged[i] = true;
        } else {
            stateChanged[i] = false;
        }
    }
}

//has the button been pressed
bool InputManager::isButtonPressed(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) {
        return false;
    }
    return buttonState[buttonIndex] && stateChanged[buttonIndex];
}

//has the button been released
bool InputManager::isButtonReleased(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) { 
        return false;
    }
    return !buttonState[buttonIndex] && stateChanged[buttonIndex];
}

//is the button currently being held
bool InputManager::isButtonDown(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) {
         return false;
    }
    return buttonState[buttonIndex];
}

//has the button been pressed and released
bool InputManager::isButtonClicked(uint8_t buttonIndex) {
    if (buttonIndex >= BUTTON_COUNT) return false;

    // Function to look for press and release
    if (clickFlag[buttonIndex] && isButtonReleased(buttonIndex)) {
        clickFlag[buttonIndex] = false;
        return true;
    }

    if (isButtonPressed(buttonIndex)) {
        clickFlag[buttonIndex] = true;
    }
    return false;    
}