#include "SM2024-Dithering.h"

#include "SM2024-Funkcje.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Pliki.h"
#include "SM2024-Konwersje.h"

void zaktualizujTabliceBayera2() {
    int zakres = 32;
    int rozmiar = 2;
    float podzial = zakres*1.0 / (rozmiar*rozmiar);

    for (int y=0; y<rozmiar; y++) {
        for (int x=0; x<rozmiar; x++) {
            zaktualizowanaTablicaBayera2[y][x] = (tablicaBayera2[y][x]*podzial)-podzial/2;
        }
    }
}
