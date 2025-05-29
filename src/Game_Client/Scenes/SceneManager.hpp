#pragma once

#include "MainMenu.hpp"
#include "GomokuScene.hpp"

// manages interactions between scenes
class SceneManager{
public:
    SceneManager(){
        gomoku = new GomokuScene(this);
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

    GomokuScene* gomoku;
    MainMenu* mainMenu;
private:
    Scene* activeScene;

};