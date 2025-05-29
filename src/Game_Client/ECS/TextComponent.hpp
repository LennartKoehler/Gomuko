#pragma once
#include "../TextureManager.hpp"
#include "Components.hpp"

class TextComponent : public Component{
private:
    RectComponent* rect;
    SDL_Texture* textTexture;
    SDL_Rect srcRect, destRect;

public:
    TextComponent() = default;
    TextComponent(std::string text){
        setTexture(text);
    }

    void setTexture(std::string text){
        textTexture = TextureManager::WriteText(text);
    }

    void init() override{
        rect = &entity->getComponent<RectComponent>(); 
        srcRect.x = srcRect.y = 0;
        srcRect.w = srcRect.h = 50;
    }


    void update() override{
        destRect.x = (int)rect->x;
        destRect.y = (int)rect->y;
        destRect.h = (int)rect->h;
        destRect.w = (int)rect->w;
    }

    void draw() override{
        TextureManager::Draw(textTexture, srcRect, destRect);
    }
    
};