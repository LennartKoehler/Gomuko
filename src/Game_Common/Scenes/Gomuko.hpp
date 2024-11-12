#pragma once
#include "Scene.hpp"
#include "../GameState.hpp"

class SceneManager;

class Gomuko : public Scene{
public:
    Gomuko(SceneManager* sceneManager);
    ~Gomuko();

    GameState* gameState;
};