#include "ChatLayer.hpp"

extern const Uint32 TEXT_RECIEVED_EVENT;


// make buttoncomponent, that when pressed executes the given function? -> e.g. set a value
ChatLayer::ChatLayer(){
    Entity& chatTextFieldEntity = manager.addEntity(); // TODO well this is just unnecessary
    chatTextFieldEntity.addComponent<RectComponent>(600,800,100,500);
    chatTextFieldEntity.addComponent<FunctionComponent>();
    chatTextFieldEntity.addComponent<TextFieldComponent>("chat", textures::tile, 28);
    chatTextFieldEntity.getComponent<TextFieldComponent>().addAction(SDLK_RETURN, std::bind(&ChatLayer::setReturn, this));
    chatTextFieldEntity.addGroup(groupButtons);
    chatTextField = &chatTextFieldEntity;

    Entity& chatHistoryEntity = manager.addEntity(); // TODO well this is just unnecessary
    chatHistoryEntity.addComponent<RectComponent>(600,700,100,500);
    chatHistoryEntity.addComponent<FunctionComponent>();
    chatHistoryEntity.addComponent<MultilineTextComponent>(28);
    chatHistoryEntity.addGroup(groupButtons);
    chatHistory = &chatHistoryEntity;
}

void ChatLayer::onEvent(Event& event){
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(ChatLayer::onMouseButtonPressed));
    dispatcher.dispatch<KeyDownEvent>(HZ_BIND_EVENT_FN(ChatLayer::onKeyDown)); // currently only for backspace
    dispatcher.dispatch<TextInputEvent>(HZ_BIND_EVENT_FN(ChatLayer::onTextInput));
    dispatcher.dispatch<TextEvent>(HZ_BIND_EVENT_FN(ChatLayer::onTextRecieved));

}

bool ChatLayer::onTextInput(TextInputEvent& event){ // TODO i should make a textfield entity that takes all kinds of events, so this isnt on the layer 
    auto& tf = chatTextField->getComponent<TextFieldComponent>();
    if (tf.isFocused()) {
        tf.addLetter(event.text);
        return true;
    }
    return false;
}

bool ChatLayer::onTextRecieved(TextEvent& event){
    chatHistory->getComponent<MultilineTextComponent>().addText(std::string{event.text});
    return true;
}

bool ChatLayer::onKeyDown(KeyDownEvent& event){
    auto& tf = chatTextField->getComponent<TextFieldComponent>();
    if (tf.isFocused()) {
        tf.keyInput(event.key);
        return true;
    }
    return false;
}

bool ChatLayer::onMouseButtonPressed(MouseButtonPressedEvent& event){
    Entity* entity = getEntityAtPosition(event.x, event.y, groupButtons);
    if (entity != nullptr){
        entity->getComponent<FunctionComponent>().executeFunction();
        return true;
    }
    return false;
}

void ChatLayer::setReturn(){
    sendChatMessageNetworkEvent(chatTextField->getComponent<TextFieldComponent>().getText());
    chatTextField->getComponent<TextFieldComponent>().reset();
}