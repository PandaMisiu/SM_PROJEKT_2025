#include "SM2024-Zmienne.h"

#include <SDL2/SDL.h>
#include "SM2024-Funkcje.h"
#include "SM2024-Pliki.h"
#include "SM2024-Konwersje.h"
#include "SM2024-Dithering.h"

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

int tablicaBayera2[2][2] = {{2,4}, {3,1}};
float zaktualizowanaTablicaBayera2[2][2];

float zaktualizowanaTablicaBayera2G[2][2];

// DCT
int maksDCT = -9999;
Uint16 dzielnik = 2;
