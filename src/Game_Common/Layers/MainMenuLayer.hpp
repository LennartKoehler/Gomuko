#pragma once
#include "PieceLayer.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"

using ButtonCallback = std::function<void()>;
class MainMenuLayer : public Layer{
public:
    MainMenuLayer();
    void onEvent(Event& event);
    bool onMouseButtonPressed(MouseButtonPressedEvent& event);
    void setGomokuButtonCallback(ButtonCallback cb);
private:
    enum groupLabels : std::size_t{
        groupButtons
    };
    ButtonCallback onGomokuButton;
};