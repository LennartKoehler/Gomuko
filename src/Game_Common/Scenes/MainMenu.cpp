#include "MainMenu.hpp"
#include "SceneManager.hpp"

MainMenu::MainMenu(SceneManager* sceneManager) : Scene(sceneManager){
    mainMenuLayer = new MainMenuLayer();
    layerStack.addLayer(mainMenuLayer);
    mainMenuLayer->setGomokuButtonCallback([sceneManager]() {
        sceneManager->setActiveScene(sceneManager->gomoku);
    });
}


void MainMenu::update(){
    layerStack.updateLayers();
}