#include "MainMenuLayer.hpp"

// make buttoncomponent, that when pressed executes the given function? -> e.g. set a value
MainMenuLayer::MainMenuLayer(){
    Entity& backgroundImage(manager.addEntity());
    backgroundImage.addComponent<RectComponent>(0,0,500,500);
    backgroundImage.addComponent<SpriteComponent>(textures::background);
    
    Entity& startGameButton(manager.addEntity());
    startGameButton.addComponent<RectComponent>(50,50,200,200);
    startGameButton.addComponent<SpriteComponent>(textures::start_game);
    startGameButton.addGroup(groupButtons);
}

void MainMenuLayer::onEvent(Event& event){
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(MainMenuLayer::onMouseButtonPressed));
}

bool MainMenuLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){

    Entity* button = getEntityAtPosition(event.x, event.y, groupButtons);
    if (button != nullptr){
        startGomuko=true;
    }
    return true;
}