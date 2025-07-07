#pragma once
#include "PieceLayer.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"
#include "../Events/TextInputEvent.hpp"
#include "../Events/KeyDownEvent.hpp"
#include "../Events/TextEvent.hpp"
#include "../ChatUtils.h"

using ButtonCallback = std::function<void()>;
using ButtonCallbackString = std::function<void(std::string)>;

class ChatLayer : public Layer{
public:
    ChatLayer();
    void onEvent(Event& event);
    bool onMouseButtonPressed(MouseButtonPressedEvent& event);
    bool onTextInput(TextInputEvent& event);
    bool onKeyDown(KeyDownEvent& event);
    bool onTextRecieved(TextEvent& event);
    void setConnectServerButtonCallback(ButtonCallbackString cb);
private:
    void setReturn();
    enum groupLabels : std::size_t{
        groupButtons,
        textFields
    };
    Entity* chatTextField;
    Entity* chatHistory;

};