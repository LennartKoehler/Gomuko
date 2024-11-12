#pragma once
#include "PieceLayer.hpp"
#include "../Events/MouseButtonPressedEvent.hpp"

class MainMenuLayer : public Layer{
public:
    MainMenuLayer();
    void onEvent(Event& event);
    bool onMouseButtonPressed(MouseButtonPressedEvent& event);
    bool startGomuko;
private:
    enum groupLabels : std::size_t{
        groupButtons
    };
};