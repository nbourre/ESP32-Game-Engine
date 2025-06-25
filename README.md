# The ESP Device Game Engine (EDGE) - Rewrite

## About This Rewrite

This repository is a comprehensive rewrite of the original [ESP32-Game-Engine by Nicolas Bourré](https://github.com/nbourre/ESP32-Game-Engine). While building on its core concepts, this version introduces significant architectural changes to improve flexibility, modularity, and testability.

### Key Changes in this Rewrite

*   **Core Architectural Refactoring (Dependency Injection):** The engine is now decoupled from specific hardware initializations. Instead of creating its own `U8G2` display object, the `EDGE` engine and `Renderer` now receive a pointer to an already-initialized `U8G2` object. This makes the engine more flexible and easier to integrate into existing projects.

*   **Advanced Scene Management:** The scene manager has been completely overhauled.
    *   **Factory-Based Creation:** Scenes are now registered with a name and created via a factory function (`registerScene`).
    *   **Scene Stack:** The manager now operates as a proper stack, with `pushScene` and `popScene` methods.
    *   **Lifecycle Hooks:** Scenes now have `onEnter()` and `onExit()` methods, which are called automatically during scene transitions.

*   **Reworked Input System:** The `InputManager` is now uses generic enums (`EDGE_Button`, `EDGE_Event`) and a deferred action queue to process inputs.

*   **Integrated Logging:** A flexible, callback-based logging system (`EDGELogger`) has been integrated throughout the engine, making debugging significantly easier.

*   **Dependency & Example Updates:**
    *   The `OneButton` and `SafeString` libraries are no longer dependencies.
    *   **The original examples (`BouncingBall`, `FlappyBird`, etc.) have been removed** as they are incompatible with the new architecture.

---

# Table of content <!-- omit in toc -->

- [The ESP Device Game Engine (EDGE)](#the-esp-device-game-engine-edge)
  - [Features](#features)
  - [Project Structure](#project-structure)
  - [Contributing](#contributing)
  - [License](#license)
  - [Credits](#credits)

---

## Features
- **Decoupled architecture** using dependency injection.
- **Advanced scene management** with a scene stack, factories, and lifecycle hooks.
- **Abstracted input system** to cleanly handle user input.
- **Customizable renderer** for any `U8g2`-compatible display.
- **Integrated callback-based logger** for easy debugging.
- **Optimized for embedded systems** with a focus on memory efficiency.

---

## Project Structure
```
EDGE/
├── src/
│   ├── EDGE.h / EDGE.cpp              # Core engine, ties components together
│   ├── Scene.h / Scene.cpp            # Base class for all scenes
│   ├── SceneManager.h / SceneManager.cpp  # Handles scene stack, factories, and transitions
│   ├── Renderer.h / Renderer.cpp      # U8g2-based graphics rendering
│   ├── InputManager.h / InputManager.cpp  # Abstracted button/input handling
│   ├── DisplayConfig.h                # Configuration for display settings
├── README.md                          # Documentation
├── platformio.ini                     # PlatformIO project configuration
```

---

## Contributing
- **Fork the repo** and submit **pull requests**.
- **Report bugs or suggest features** via GitHub issues.
- **Share your own scenes/games** using EDGE.

---

## License
This project is open-source under the **MIT License**.

The original work by **Nicolas Bourré** is also licensed under the MIT License. This rewritten version is a derivative work and is therefore also provided under the same license terms. See the [LICENSE](LICENSE) file for more details.

---

## Credits
- Original engine concept and implementation by **Nicolas Bourré**.
- This version rewritten and refactored by **benda95280**.
