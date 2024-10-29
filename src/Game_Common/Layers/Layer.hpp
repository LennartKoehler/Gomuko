#pragma once
#include "Layerinclude.hpp"


class Layer{
public:
    virtual void draw(){
        manager.draw();
    }

    virtual void update(){
        manager.update();
    }

    virtual void refresh(){
        manager.refresh();
    }
protected:
    Manager manager;
};
