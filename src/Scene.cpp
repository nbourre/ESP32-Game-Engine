#include "Scene.h"
#include <Arduino.h> 
#include <stdarg.h> // For va_list, etc.

// Define the static logger member
EDGELogger Scene::_masterLogger = nullptr;

Scene::~Scene() {
}

void Scene::init() { 
    managesOwnDrawing = false; 
}

void Scene::update(unsigned long deltaTime) {
    //Empty
}

void Scene::draw(Renderer& renderer) { 
    //Empty
}
