#ifndef SM2024_KONWERSJE_H_INCLUDED
#define SM2024_KONWERSJE_H_INCLUDED

#include <SDL2/SDL.h>
#include "SM2024-Zmienne.h"

SDL_Color YCbCrToRGB(YCbCr ycbcr);
YCbCr RGBToYCbCr(SDL_Color rgb);

void setRGB555(int xx,int yy,Uint8 r,Uint8 g,Uint8 b);
void setRGB555_(int xx,int yy,Uint16 rgb555);
SDL_Color getRGB555(int xx, int yy);
Uint16 getRGB555_(int xx, int yy);

void YCbCrSubSampling();

Uint8 konwertujNaSzary(SDL_Color color);

#endif // SM2024-KONWERSJE_H_INCLUDED
