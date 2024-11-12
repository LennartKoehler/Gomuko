#pragma once

#include "Scene.hpp"

class SceneManager;

class MainMenu : public Scene{
public:
    MainMenu(SceneManager* sceneManager);
    void update() override;

private:
    MainMenuLayer* mainMenuLayer;
};