#ifndef SM2024_ZMIENNE_H_INCLUDED
#define SM2024_ZMIENNE_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>

using namespace std;

#define szerokosc 640
#define wysokosc 400

#define M_PI 3.14159265358979323846

#define tytul "SM2024 - Projekt - Zespol 13"

extern SDL_Window* window;
extern SDL_Surface* screen;

struct YCbCr {
    float y;
    float cb;
    float cr;

    YCbCr(float y,float cb,float cr){
        this->y = y;
        this->cb = cb;
        this->cr = cr;
    }

    YCbCr(){}
};

extern int tablicaBayera2[2][2];
extern float zaktualizowanaTablicaBayera2[2][2];
extern float zaktualizowanaTablicaBayera2G[2][2];

// DCT
const int rozmiarBloku = 8;

struct macierz {
    float dct[rozmiarBloku] [rozmiarBloku];
    Uint8 dane[rozmiarBloku] [rozmiarBloku];
};

extern int maksDCT;
extern Uint16 dzielnik;

enum ColorTypes {
    RGB888_TYPE,
    RGB555_TYPE,
    YCbCr_TYPE
};



#endif // SM2024_ZMIENNE_H_INCLUDED
