#include "LayerStack.hpp"

LayerStack::~LayerStack(){
    for (Layer* layer : layers){
        delete layer;
    }
}

void LayerStack::handleEvent(Event& event){
    EventDispatcher dispatcher(event);
    for (Layer* layer : layers){
        if(event.handled){
            break;
        }
        layer->onEvent(event);
    }
}

void LayerStack::addLayer(Layer* layer){
    layers.emplace_back(layer);
}

void LayerStack::updateLayers(){
    for (Layer* layer : layers){
        layer->update();
    }
}

void LayerStack::refreshLayers(){
    for (Layer* layer : layers){
        layer->refresh();
    }
}

void LayerStack::drawLayers(){
    for (Layer* layer : layers){
        layer->draw();
    }
}