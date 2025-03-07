// podstawowe funkcje
#ifndef SM2024_FUNKCJE_H_INCLUDED
#define SM2024_FUNKCJE_H_INCLUDED

#include <SDL2/SDL.h>
#include "SM2024-Kompresja.h"


void Funkcja1();
void Funkcja2();
void Funkcja3();
void Funkcja4();
void Funkcja5();
void Funkcja6();
void Funkcja7();
void Funkcja8();
void Funkcja9();
void Funkcja10();

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);
SDL_Color getPixel(int x, int y);
void czyscEkran(Uint8 R, Uint8 G, Uint8 B);
void ladujBMP(char const* nazwa, int x, int y);



#endif // SM2024_FUNKCJE_H_INCLUDED
