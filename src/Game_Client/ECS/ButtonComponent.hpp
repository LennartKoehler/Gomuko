#pragma once
#include "Components.hpp"


class ButtonComponent : public Component{
private:
    RectComponent* rect;
    SDL_Rect srcRect, destRect;
    std::function<void()> func;
public:
    ButtonComponent() = default;
    ButtonComponent(std::function<void()> func) : func(func){
    }



    void init() override{
        rect = &entity->getComponent<RectComponent>(); 

        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 50;       
    }



};