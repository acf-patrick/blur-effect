#include "blur.h"
#include <SDL2/SDL_image.h>
#include <iostream>

SDL_Surface* boxBlur(const std::string& file_name, int extent)
{
    return boxBlur(IMG_Load(file_name.c_str()), extent);
}

SDL_Surface* boxBlur(SDL_Surface* surface, int extent)
{
    if (extent <= 0 or !surface)
        return surface;

    SDL_LockSurface(surface);

    int n = extent*2 + 1;
    int w = surface->w, h = surface->h;
    SDL_Rect sr = { 0, 0, w, h };
    SDL_Surface* ret = SDL_CreateRGBSurface(SDL_SWSURFACE, w, h, 32, RMASK, GMASK, BMASK, AMASK);
    if (!ret)
    {
        std::cerr << "Can't create destination surface!\n";
        return NULL;
    }

    for (int i = 0; i < w; ++i)
    {
        for (int j = 0; j < h; ++j)
        {
            int r, g, b, a;
            r = g = b = a = 0;
            for (int x = -extent; x <= extent; ++x)
            for (int y = -extent; y <= extent; ++y)
            {
                SDL_Point p = { x+i, y+j };
                if (SDL_PointInRect(&p, &sr))
                {
                    Uint32 px = getPixel(p.x, p.y, surface);
                    Uint8 r0, g0, b0, a0;
                    SDL_GetRGBA(px, surface->format, &r0, &g0, &b0, &a0);
                    r += r0;
                    g += g0;
                    b += b0;
                    a += a0;
                }
            }
            r /= n*n;
            g /= n*n;
            b /= n*n;
            a /= n*n; 
            setPixel(i, j, SDL_MapRGBA(ret->format, r, g, b, a), ret);
        }
    }

    return ret;
}

Uint32 getPixel(int x, int y, SDL_Surface* surface)
{
    if (!surface)
        return 0;
    if (x<0 or y<0 or x>=surface->w or y>=surface->h)
        return 0;

    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        return *p;
    case 2:
        return *(Uint16*)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0]<<16 | p[1]<<8 | p[2];
        else
            return p[0] | p[1]<<8 | p[2]<<16;
    case 4:
        return *(Uint32*)p;
    default : ;
    }

    return 0;
}

void setPixel(int x, int y, Uint32 pixel, SDL_Surface* surface)
{
    if (!surface)
        return;
    if (x<0 or y<0 or x>=surface->w or y>=surface->h)
        return;

    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
    case 1:
        *p = pixel;
        break;
    case 2:
        *(Uint16*)p = pixel;
        break;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
        {
            p[0] = (pixel >> 16) & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = pixel & 0xff;
        }
        else
        {
            p[0] = pixel & 0xff;
            p[1] = (pixel >> 8) & 0xff;
            p[2] = (pixel >> 16 ) & 0xff;
        }
        break;
    case 4:
        *(Uint32*)p = pixel;
        break;
    default : ;
    }
}