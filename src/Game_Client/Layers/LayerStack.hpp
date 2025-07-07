#pragma once

#include "PieceLayer.hpp"
#include "BackgroundLayer.hpp"
#include "MainMenuLayer.hpp"
#include "ChatLayer.hpp"


class LayerStack{
public:
    LayerStack(){}
    ~LayerStack();
    void addLayer(Layer* layer);
    void updateLayers();
    void refreshLayers();
    void drawLayers();
    void handleEvent(Event& event);

private:
    std::vector<Layer*> layers;
};