#pragma once

#include <string>
#include <SDL2/SDL.h>

// conveniency
class Surface
{
    SDL_Surface* _data = nullptr;

public:
    Surface(const std::string&);
    ~Surface();

// pixel manipulation 
    Uint32 get(int, int);
    void   set(int, int);
};

// blur file image
// using box algorithm
SDL_Texture* boxBlur(const std::string&, int extent = 1);

// blur surface
// using box algorithm
SDL_Texture* boxBlur(SDL_Surface*, int extent = 1);

// blur file image
// using Gauss algorithm
SDL_Texture* gaussianBlur(const std::string&, int extent = 1);

// blur surface
// using Gauss algorithm
SDL_Texture* gaussianBlur(SDL_Surface*, int extent = 1);