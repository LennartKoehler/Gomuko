#include "ChatLayer.hpp"

// make buttoncomponent, that when pressed executes the given function? -> e.g. set a value
ChatLayer::ChatLayer(int size){
    Entity& backgroundImage(manager.addEntity());
    backgroundImage.addComponent<RectComponent>(0,0,size,size);
    backgroundImage.addComponent<SpriteComponent>(textures::background);

    Entity& startGameButton(manager.addEntity());
    startGameButton.addComponent<RectComponent>(200,200,50,100);
    startGameButton.addComponent<SpriteComponent>(textures::start_game);
    startGameButton.addGroup(groupButtons);
}

void ChatLayer::onEvent(Event& event){
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ChatLayer::onMouseButtonPressed));
}

bool ChatLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){

    Entity* button = getEntityAtPosition(event.x, event.y, groupButtons);
    if (button != nullptr){
        onGomokuButton();
    }
    return true;
}

void ChatLayer::setGomokuButtonCallback(ButtonCallback cb){
    onGomokuButton = std::move(cb);
}
