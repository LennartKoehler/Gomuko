#include "MainMenu.hpp"
#include "SceneManager.hpp"

MainMenu::MainMenu(SceneManager* sceneManager) : Scene(sceneManager){
    mainMenuLayer = new MainMenuLayer();
    layerStack.addLayer(mainMenuLayer);
}

void MainMenu::update(){
    if ( mainMenuLayer->startGomuko ){
        sceneManager->setActiveScene(sceneManager->gomuko);
    }
    layerStack.updateLayers();
}