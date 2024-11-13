#pragma once

#include "MainMenu.hpp"
#include "Gomuko.hpp"

// manages interactions between scenes
class SceneManager{
public:
    SceneManager(){
        gomuko = new Gomuko(this);
        mainMenu = new MainMenu(this);
        activeScene = mainMenu;
    }
    Scene* getActiveScene(){ return activeScene; }
    void setActiveScene(Scene* activeScene) {
        this->activeScene = activeScene;
        activeScene->refresh();
        activeScene->update();
        activeScene->draw();
        }

    Gomuko* gomuko;
    MainMenu* mainMenu;
private:
    Scene* activeScene;

};