#include <stdio.h>
#include <math.h>
#include <fstream>
#include <SDL2/SDL.h>
#include "SM2024-Funkcje.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Pliki.h"
#include "SM2024-Konwersje.h"
#include "SM2024-Filtry.h"

// TYP 3 - tylko dla bezstratnej
/*
vector<vector<SDL_Color>> filtrUsredniajacyRGB(){
    vector<vector<SDL_Color>> filtr(szerokosc/2,wysokosc/2);

    for (int y = 0; y < wysokosc/2; y ++) {
        for (int x = 0; x < szerokosc/2; x ++) {
            SDL_Color pixel1 = getPixel(x,y);
            SDL_Color pixel2 = getPixel(x,y-1); // gora
            SDL_Color pixel3 = getPixel(x-1,y); // lewo

            SDL_Color rgb;

            rgb.r = pixel1.r-((pixel2.r+pixel3.r)/2);
            rgb.g = pixel1.g-((pixel2.g+pixel3.g)/2);
            rgb.b = pixel1.b-((pixel2.b+pixel3.b)/2);

            filtr[x][y] = {rgb.r,rgb.g,rgb.b};
        }
    }
    return filtr;
}

void odwrocFiltrUsredniajacyRGB(const vector<vector<SDL_Color>>& filtr) {
    for (int y = 0; y < wysokosc / 2; y++) {
        for (int x = 0; x < szerokosc / 2; x++) {
            SDL_Color rgb = filtr[x][y];

            SDL_Color pixel2 = getPixel(x, y - 1); // gora
            SDL_Color pixel3 = getPixel(x - 1, y); // lewo

            SDL_Color original;

            original.r = rgb.r + ((pixel2.r + pixel3.r) / 2);
            original.g = rgb.g + ((pixel2.g + pixel3.g) / 2);
            original.b = rgb.b + ((pixel2.b + pixel3.b) / 2);

            setPixel(x, y, original);
        }
    }
}*/
