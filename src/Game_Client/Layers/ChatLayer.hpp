#pragma once
#include "Layer.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"
#include "../Textures.hpp"

#include <functional>

using ButtonCallback = std::function<void()>;
class ChatLayer : public Layer{
public:
    ChatLayer(int size);
    void onEvent(Event& event);
    bool onMouseButtonPressed(MouseButtonPressedEvent& event);
    void setGomokuButtonCallback(ButtonCallback cb);
private:
    enum groupLabels : std::size_t{
        groupButtons
    };
    ButtonCallback onGomokuButton;

};