#pragma once

#include "Components.hpp"

struct RectComponent : public Component{

    float x;
    float y;
    float h;
    float w;

    RectComponent(){
    }

    RectComponent(float x, float y, float height, float width)
        : x(x), y(y), h(height), w(width){
    }

    void update() override{}
    void draw() override{}
    void init(){}
};