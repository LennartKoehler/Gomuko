#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
class Game;
class TextureManager{

public:

    static SDL_Texture* LoadTexture(const char* fileName);
    static SDL_Texture* WriteText(const char* text, int fontsize);
    static SDL_Texture* WriteText(std::string text, int fontsize);
    static SDL_Texture* WriteTextOnBackground(const char* text, SDL_Surface* background);
    static SDL_Texture* WriteTextWrapped(std::string text, int fontsize, int wrapLength);
    static SDL_Texture* WriteTextWrapped(const char* text, int fontsize, int wrapLength);


    static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);

};