// funkcje do operacji na plikach
#ifndef SM2024_PLIKI_H_INCLUDED
#define SM2024_PLIKI_H_INCLUDED

#define szerokosc 640
#define wysokosc 400

#include <iostream>
#include <SDL2/SDL.h>


std::ofstream zapiszDoPliku(Uint16 szerokoscObrazka, Uint16 wysokoscObrazka, Uint8 tryb, Uint8 dithering, Uint8 kompresja);
std::ifstream odczytPliku(Uint16 &szerokoscObrazka, Uint16 &wysokoscObrazka, Uint8 &tryb, Uint8 &dithering, Uint8 &kompresja);

// [POMOCNICZE]
void saveDCTBlocks(std::ofstream& wyjscie, std::vector<macierz> blokDCTVector);
void findMaxDivider();
std::vector<macierz> readDCTBlocks(std::ifstream& wejscie,int liczbaBlokow, int odczytanyDzielnik);
// ---

void saveToFileByteRun_COLOR(std::ofstream& wyjscie, Uint8 A_tab[], Uint8 B_tab[], Uint8 C_tab[], int dlugosc);
void saveToFileByteRun_GRAY(std::ofstream& wyjscie, Uint8 GRAY_tab[], int dlugosc);

void saveToFileDCT_COLOR(std::ofstream& wyjscie,ColorTypes colorType);
void saveToFileDCT_GRAY(std::ofstream& wyjscie,ColorTypes colorType);

void readFromFileByteRun_COLOR(std::ifstream& wejscie, ColorTypes colorType, bool isDithering);
void readFromFileByteRun_GRAY(std::ifstream& wejscie, ColorTypes colorType, bool isDithering);


void readFromFileDCT_COLOR(std::ifstream& wejscie, ColorTypes colorType, bool isDithering);
void readFromFileDCT_GRAY(std::ifstream& wejscie, ColorTypes colorType, bool isDithering);

void RGB888ToFile(Uint8 pickedTryb, Uint8 pickedKompresja);
void RGB888FromFile();

void YCbCr888ToFile(Uint8 pickedTryb, Uint8 pickedKompresja);
void YCbCr888FromFile();

void RGB555ToFile(Uint8 pickedTryb, bool isDithering, Uint8 pickedKompresja);
void RGB555FromFile();

#endif // SM2024_PLIKI_H_INCLUDED
