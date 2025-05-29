#pragma once
#include "PieceLayer.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"
#include "../Events/TextInputEvent.hpp"
#include "../Events/KeyDownEvent.hpp"


using ButtonCallback = std::function<void()>;
using ButtonCallbackString = std::function<void(std::string)>;

class MainMenuLayer : public Layer{
public:
    MainMenuLayer();
    void onEvent(Event& event);
    bool onMouseButtonPressed(MouseButtonPressedEvent& event);
    bool onTextInput(TextInputEvent& event);
    bool onKeyDown(KeyDownEvent& event);
    void setGomokuButtonCallback(ButtonCallback cb);
    void setConnectServerButtonCallback(ButtonCallbackString cb);
private:
    enum groupLabels : std::size_t{
        groupButtons,
        textFields
    };
    Entity* startGameButton;
    Entity* connectServerButton;
    Entity* serverAddressTextField;

};