#pragma once

#include "MainMenu.hpp"
#include "Gomuko.hpp"

// manages interactions between scenes
class SceneManager{
public:
    SceneManager(){}
    Scene* getActiveScene(){ return activeScene; }
    void setActiveScene(Scene* activeScene) { this->activeScene = activeScene; }

    Gomuko* gomuko = new Gomuko(this);
    MainMenu* mainMenu = new MainMenu(this);
    Scene* activeScene = mainMenu;

};