#ifndef SM2024-KOMPRESJA_H_INCLUDED
#define SM2024-KOMPRESJA_H_INCLUDED

using namespace std;
#include <vector>
#include "SM2024-Kompresja.h"
#include "SM2024-Zmienne.h"


vector<Sint8> byteRunKompresja(Uint8 wejscie[],int dlugosc);
vector<Uint8> byteRunDekompresja(vector<Sint8> wejscie,int dlugosc);


// DCT
int normalizacja(vector<macierz> &blokDCTVector);
macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]);
macierz idct(float DCT[rozmiarBloku][rozmiarBloku]);

#endif // SM2024-KOMPRESJA_H_INCLUDED
