#pragma once

#include "Scene.hpp"

class SceneManager;

class MainMenu : public Scene{
public:
    MainMenu(SceneManager* sceneManager);
    void update() override;
    void sendIPAddressEvent(const char* ipaddress);

private:
    MainMenuLayer* mainMenuLayer;
};