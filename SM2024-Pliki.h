// funkcje do operacji na plikach
#ifndef SM2024_PLIKI_H_INCLUDED
#define SM2024_PLIKI_H_INCLUDED

#define szerokosc 640
#define wysokosc 400

#include <iostream>
#include <SDL2/SDL.h>


std::ofstream zapiszDoPliku(Uint16 szerokoscObrazka, Uint16 wysokoscObrazka, Uint8 tryb, Uint8 dithering, Uint8 kompresja);
std::ifstream odczytPliku(Uint16 &szerokoscObrazka, Uint16 &wysokoscObrazka, Uint8 &tryb, Uint8 &dithering, Uint8 &kompresja);

void RGB888ToFile(Uint8 pickedTryb, Uint8 pickedKompresja);
void RGB888FromFile();

void YCbCr888ToFile(Uint8 pickedTryb, Uint8 pickedKompresja);
void YCbCr888FromFile();

void RGB555ToFile(Uint8 pickedTryb, bool isDithering, Uint8 pickedKompresja);
void RGB555FromFile();

#endif // SM2024_PLIKI_H_INCLUDED
