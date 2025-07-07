#pragma once
#include "TextComponent.hpp"

class TextHelper{
public:
    TextHelper(std::string text, SDL_Texture* texture, SDL_Rect rect) : text(text), texture(texture), rect(rect){}

    std::string text;
    SDL_Texture* texture;
    SDL_Rect rect; // relative position
};


class MultilineTextComponent : public Component{
public:
    MultilineTextComponent(std::vector<std::string> textLines, int fontsize, int wraplength, int yOffset) : wraplength(wraplength), yOffset(yOffset), fontsize(fontsize){
        for (auto string : textLines){
            addText(string);
        }
    }
    MultilineTextComponent(int fontsize) : fontsize(fontsize){}

    void addText(std::string text){
        SDL_Rect rect;

        SDL_Texture* textTexture = TextureManager::WriteTextWrapped(text, fontsize, wraplength);

        SDL_QueryTexture(textTexture, NULL, NULL, &rect.w, &rect.h);
        rect.x = 0;
        rect.y = yOffset;
        yOffset -= rect.h;

        texts.emplace_back(TextHelper(text, textTexture, rect));
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

    void draw(){
        for (auto text: texts){
            SDL_Rect outRect {destRect.x + text.rect.x, destRect.y + yOffset - text.rect.y, text.rect.w, text.rect.h};
            TextureManager::Draw(text.texture, srcRect, outRect);
        }
    }



private:
    int yOffset = 0;
    int wraplength;
    int fontsize;
    std::vector<TextHelper> texts;
    SDL_Rect srcRect, destRect;
    RectComponent* rect;


};