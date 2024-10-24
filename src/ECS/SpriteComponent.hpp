#pragma once
#include "../TextureManager.hpp"
#include "Components.hpp"

class SpriteComponent : public Component{
private:
    RectComponent* rect;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;

public:
    SpriteComponent() = default;
    SpriteComponent(const char* path){
        setTexture(path);
    }

    void setTexture(const char* path){
        texture = TextureManager::LoadTexture(path);
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
        TextureManager::Draw(texture, srcRect, destRect);
    }
    
};