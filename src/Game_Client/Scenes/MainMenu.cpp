#include "MainMenu.hpp"
#include "SceneManager.hpp"

extern const Uint32 NETWORK_CONNECTION_EVENT;

MainMenu::MainMenu(SceneManager* sceneManager) : Scene(sceneManager){
    mainMenuLayer = new MainMenuLayer();
    layerStack.addLayer(mainMenuLayer);
    mainMenuLayer->setGomokuButtonCallback([sceneManager, this]() {
        sceneManager->setActiveScene(sceneManager->gomoku);
    });
    mainMenuLayer->setConnectServerButtonCallback([sceneManager, this](std::string address) {
        auto* addressCopy = new std::string(address);
        sendIPAddressEvent(addressCopy->c_str());
    });
}

void MainMenu::sendIPAddressEvent(const char* ipaddress){
    SDL_Event event;
    SDL_zero(event);
    event.type = NETWORK_CONNECTION_EVENT;
    event.user.data1 = (void*)ipaddress;
    SDL_PushEvent(&event);

}

void MainMenu::update(){
    layerStack.updateLayers();
}