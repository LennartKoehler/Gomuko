#include "MainMenuLayer.hpp"

// make buttoncomponent, that when pressed executes the given function? -> e.g. set a value
MainMenuLayer::MainMenuLayer(){
    Entity& backgroundImage(manager.addEntity());
    backgroundImage.addComponent<RectComponent>(0,0,1000,1000);
    backgroundImage.addComponent<SpriteComponent>(textures::background);
    
    Entity& startGameButtonEntity = manager.addEntity();
    startGameButtonEntity.addComponent<RectComponent>(200,200,50,100);
    startGameButtonEntity.addComponent<TextComponent>("start game");
    startGameButtonEntity.addGroup(groupButtons);
    startGameButton = &startGameButtonEntity;

    Entity& connectServerButtonEntity = manager.addEntity();
    connectServerButtonEntity.addComponent<RectComponent>(200,400,50,100);
    connectServerButtonEntity.addComponent<TextComponent>("connect to server");
    connectServerButtonEntity.addGroup(groupButtons);
    connectServerButton = &connectServerButtonEntity;

    Entity& serverAddressTextFieldEntity = manager.addEntity(); // TODO well this is just unnecessary
    serverAddressTextFieldEntity.addComponent<RectComponent>(200,600,50,100);
    serverAddressTextFieldEntity.addComponent<TextComponent>();
    serverAddressTextFieldEntity.addComponent<FunctionComponent>();
    serverAddressTextFieldEntity.addComponent<TextFieldComponent>("ServerAddress");
    serverAddressTextFieldEntity.addGroup(groupButtons);
    serverAddressTextField = &serverAddressTextFieldEntity;

}

void MainMenuLayer::onEvent(Event& event){
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(MainMenuLayer::onMouseButtonPressed));
    dispatcher.dispatch<KeyDownEvent>(HZ_BIND_EVENT_FN(MainMenuLayer::onKeyDown)); // currently only for backspace
    dispatcher.dispatch<TextInputEvent>(HZ_BIND_EVENT_FN(MainMenuLayer::onTextInput));
}

bool MainMenuLayer::onTextInput(TextInputEvent& event){
    serverAddressTextField->getComponent<TextFieldComponent>().addLetter(event.text);
    return true;
}

bool MainMenuLayer::onKeyDown(KeyDownEvent& event){
    serverAddressTextField->getComponent<TextFieldComponent>().keyInput(event.key);
    return true;
}

bool MainMenuLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){
    Entity* entity = getEntityAtPosition(event.x, event.y, groupButtons);
    if (entity != nullptr){
        entity->getComponent<FunctionComponent>().executeFunction();
    }
    return true;
}

void MainMenuLayer::setGomokuButtonCallback(ButtonCallback cb){ //TODO this is the only reason i need them as member objects
    startGameButton->addComponent<FunctionComponent>(std::move(cb));
}

void MainMenuLayer::setConnectServerButtonCallback(ButtonCallbackString cb){ // TODO this make the mainmenuscene and layer somewhat coupled
    auto wrapper = [this, cb](){cb(this->serverAddressTextField->getComponent<TextFieldComponent>().getText());};
    connectServerButton->addComponent<FunctionComponent>(std::move(wrapper));
}

