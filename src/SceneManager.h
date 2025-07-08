#pragma once

#include <Arduino.h> 
#include <map>
#include <vector>
#include <functional> 

#define MAX_SCENES 5

// Forward declarations
class InputManager;
class Renderer;
class Scene; 

// Define the logger type directly to break circular dependency with EDGE.h
using EDGELogger = std::function<void(const char* message)>;

using SceneFactoryFunction = std::function<Scene*(void* configData)>;


class SceneManager {
public:
    SceneManager(); 
    ~SceneManager();
    void processSceneChanges(); 

    void setInputManager(InputManager* manager);
    void setLogger(EDGELogger logger);

    bool registerScene(const String& name, SceneFactoryFunction factory);

    bool setCurrentScene(const String& sceneName, void* configData = nullptr); 
    bool pushScene(const String& sceneName, void* configData = nullptr);    
    bool popScene();

    void requestSetCurrentScene(const String& sceneName, void* configData = nullptr);
    void requestPushScene(const String& sceneName, void* configData = nullptr);

    void update(unsigned long dt);
    void draw(Renderer& rendererRef); 
    Scene* getCurrentScene() const;
    String getCurrentSceneName() const;
    SceneType getPreviousSceneType() const; // <<< NEW

    SceneFactoryFunction getFactoryByName(const String& name) const;
    std::vector<String> getRegisteredSceneNames() const;

    bool isSceneChangePending() const;
    String getPendingSceneName() const;
    void* getPendingConfigData() const;
    bool getPendingReplaceStack() const;
    void clearPendingSceneChange();


private:
    Scene* sceneStack[MAX_SCENES] = {nullptr};
    int sceneCount = 0;
    String _sceneNameStack[MAX_SCENES]; 
    SceneType _previousSceneType = SceneType::UNKNOWN; // <<< NEW

    InputManager* inputManager = nullptr;
    EDGELogger _logger;

    std::map<String, SceneFactoryFunction> _sceneFactories;

    String _pendingNextSceneName = "";
    void* _pendingConfigData = nullptr;
    bool _pendingReplaceStack = true;
    bool _pendingSceneChange = false;

    Scene* createSceneByName(const String& sceneName, void* configData); 
    void clearStack();
};