#include "blur.h"
#include <iostream>
#include <SDL2/SDL_image.h>

int main(int argc, char** argv)
{
    if (argc > 1)
    {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_CreateWindowAndRenderer(1, 1, SDL_WINDOW_SHOWN, &window, &renderer);

        SDL_Surface* s = gaussianBlur(std::string(argv[1]), (argc > 2)?std::atoi(argv[2]):1);
        if (s)
            IMG_SavePNG(s, "output.png");
        else
            std::cerr << "Unable to create image" << std::endl;
        SDL_FreeSurface(s);

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
    return 0;
}