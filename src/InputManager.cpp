#include "InputManager.h"

void InputManager::init() {
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    buttonState = false;
    stateChanged = false;
    waitTime = 0;
    clickFlag = false;
}

void InputManager::update(unsigned long dt) {
    
    // Check if in debounce time window
    if (waitTime >= dt) {
        waitTime -= dt;
        return;
    }

    waitTime = 0;
    bool reading = digitalRead(BUTTON_PIN) == LOW;
    
    if (reading != buttonState) {
        // Button pressed or released.
        waitTime = 100;
        buttonState = reading;
        stateChanged = true;
    } else {
        stateChanged = false;
    }

}

bool InputManager::isButtonPressed() {
    return buttonState & stateChanged;
}


bool InputManager::isButtonReleased() {
    return !buttonState && stateChanged;
}

bool InputManager::isButtonDown() {
    return buttonState;
}

bool InputManager::isButtonClicked() {
    // Function to look for press and release
    if (clickFlag && isButtonReleased()) {
        clickFlag = false;
        return true;
    }

    if (isButtonPressed()) {
        clickFlag = true;
    }
    return false;    
}



