# The ESP Device Game Engine (EDGE)

**EDGE** is a lightweight, **entity-based game engine** designed for **embedded devices** with **small displays**.  
It features **scene management**, **entity handling**, and is **hardware-agnostic**, working with **any U8g2-compatible display**.



https://github.com/user-attachments/assets/466554af-29f4-4d50-976b-3db33ba87524



---

## Features
- **Entity-based architecture** for modular game objects.
- **Scene management** to handle game flow.
- **Customizable renderer & input system** (supporting different devices).
- **Optimized for embedded systems** (memory-efficient, no heap fragmentation).
- **Supports any device with a U8g2-compatible display**.

---

## How It Works
EDGE is built around **scenes** and **entities**:

### Scenes
- A **scene** represents a game state (e.g., main menu, gameplay, pause screen).
- Each scene contains multiple **entities** (e.g., a player, obstacles, UI).
- The **SceneManager** handles scene transitions.

### Entities
- Each **entity** is an object in the game (player, enemy, particle, etc.).
- Entities **update every frame** and **draw themselves**.

---

## Project Structure
```
EDGE/
│── src/
│   ├── EDGE.h / EDGE.cpp        # Core engine logic
│   ├── Scene.h / Scene.cpp      # Scene management
│   ├── SceneManager.h / SceneManager.cpp  # Handles multiple scenes
│   ├── Renderer.h / Renderer.cpp # U8g2-based graphics rendering
│   ├── InputManager.h / InputManager.cpp  # Button/input handling
│── examples/
│   ├── BouncingBallScene/        # Example: Simple physics
│   ├── FlappyBirdScene/          # Example: Game using physics + input
│── README.md                     # Documentation
│── platformio.ini                 # PlatformIO project configuration
```

---

## Setup & Installation

### Hardware Requirements
- **ESP32/ESP8266/Arduino** (or any microcontroller with `U8g2` support).
- **Display:** SSD1306 OLED, SH1106, or other `U8g2`-compatible screens.
- **Buttons/Input Device** (can be GPIO buttons or a custom controller).

### Library Dependencies
Ensure you have the following libraries installed:
```sh
pio lib install "U8g2" "ArduinoQueue" "SafeString" "OneButton"
```

### Include EDGE in Your Project
In **PlatformIO**, add:
```cpp
#include "EDGE.h"
```

---

## Usage

### Initializing the Engine
Create an instance of `EDGE` and set the first scene:
```cpp
#include "EDGE.h"
#include "MyGameScene.h"

EDGE engine;
MyGameScene gameScene;

void setup() {
    engine.init();
    engine.setScene(&gameScene);
}

void loop() {
    engine.update();
    engine.draw();
}
```

---

### Creating a Scene
A **scene** manages all game objects.

**Example: `MyGameScene.h`**
```cpp
#pragma once
#include "Scene.h"

class MyGameScene : public Scene {
public:
    void init() override;
    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;
};
```

**Example: `MyGameScene.cpp`**
```cpp
#include "MyGameScene.h"
#include "EDGE.h"

extern EDGE engine;

void MyGameScene::init() {
    // Initialize scene
}

void MyGameScene::update(unsigned long deltaTime) {
    // Game logic
}

void MyGameScene::draw(Renderer& renderer) {
    renderer.drawText(10, 10, "Hello EDGE!");
}
```

---

### Creating an Entity
Entities **move, interact, and render themselves**.

**Example: `Player.h`**
```cpp
#pragma once
#include "Entity.h"

class Player : public Entity {
public:
    Player();
    void update(unsigned long deltaTime) override;
    void draw(Renderer& renderer) override;

private:
    float x, y, velocity;
};
```

**Example: `Player.cpp`**
```cpp
#include "Player.h"

Player::Player() : x(10), y(20), velocity(0) {}

void Player::update(unsigned long deltaTime) {
    velocity += 0.1;  // Gravity effect
    y += velocity;
}

void Player::draw(Renderer& renderer) {
    renderer.drawCircle(x, y, 3);
}
```

---

### Customizing the Renderer
The **renderer** is hardware-agnostic. Modify it to support **any U8g2-compatible display**.

**Example: `Renderer.h`**
```cpp
#pragma once
#include <U8g2lib.h>

class Renderer {
public:
    Renderer();
    void init();
    void drawText(int x, int y, const char* str);
    void drawCircle(int x, int y, int radius);
    void setFont(const uint8_t* font);
};
```

**Example: `Renderer.cpp`**
```cpp
#include "Renderer.h"

Renderer::Renderer() : u8g2(U8G2_R0, U8X8_PIN_NONE, 6, 5) {}

void Renderer::init() {
    u8g2.begin();
    u8g2.setFont(u8g2_font_6x10_tf);
}

void Renderer::drawText(int x, int y, const char* str) {
    u8g2.drawStr(x, y, str);
}

void Renderer::drawCircle(int x, int y, int radius) {
    u8g2.drawDisc(x, y, radius);
}
```

---

### Customizing Input Handling
Modify **InputManager** to work with **any device**.

**Example: `InputManager.h`**
```cpp
#pragma once
#include <OneButton.h>

#define BUTTON_PIN 9  // Change if needed

class InputManager {
public:
    InputManager();
    void init();
    bool isButtonPressed();
};
```

**Example: `InputManager.cpp`**
```cpp
#include "InputManager.h"

OneButton button(BUTTON_PIN, true);

InputManager::InputManager() {}

void InputManager::init() {
    button.attachClick([]() {
        // Handle button click
    });
}

bool InputManager::isButtonPressed() {
    button.tick();
    return button.isPressed();
}
```

---

## Example Projects
- **Bouncing Ball** (Basic physics test)
- **Flappy Bird** (Button-controlled physics)
- **Tetris (Coming soon)**

---

## Customization Guide
- **Want to use a different screen?** Modify `Renderer.cpp` to suit your display.
- **Need different controls?** Change `InputManager.cpp` to work with your buttons.
- **Adding new scenes?** Extend `Scene` and implement `update()` and `draw()`.

---

## Contributing
- **Fork the repo** and submit **pull requests**.
- **Report bugs or suggest features** via GitHub issues.
- **Share your own scenes/games** using EDGE.

---

## License
EDGE is **open-source** under the **MIT License**.

---

## Credits
Developed by **Nicolas Bourré** as a modular **game engine for embedded systems**.
