#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>

#include <fstream>
#include <vector>

#include "SM2024-Dithering.h"
#include "SM2024-Filtry.h"
#include "SM2024-Funkcje.h"
#include "SM2024-Kompresja.h"
#include "SM2024-Konwersje.h"
#include "SM2024-Pliki.h"
#include "SM2024-Zmienne.h"

vector<Sint8> byteRunKompresja(Uint8 wejscie[], int dlugosc) {
    int i = 0;
    vector<Sint8> tab;

    while (i < dlugosc - 1) {
        if ((i < dlugosc - 1) && (wejscie[i] == wejscie[i + 1])) {
            int j = 0;
            while ((i + j < dlugosc - 1) && (wejscie[i + j] == wejscie[i + 1 + j]) && (j < 127)) j++;

            tab.push_back(-j);
            tab.push_back(wejscie[i + j] - 128);

            i += (j + 1);
        } else {
            int j = 0;

            while ((i + j < dlugosc - 1) && (wejscie[i + j] != wejscie[i + 1 + j]) && (j < 127)) j++;

            if ((i + j == dlugosc - 1) && (j < 128)) j++;

            tab.push_back(j - 1);
            for (int k = 0; k < j; k++) tab.push_back(wejscie[i + k] - 128);

            i += j;
        }
    }
    return tab;
}

vector<Uint8> byteRunDekompresja(vector<Sint8> wejscie, int dlugosc) {
    int i = 0;
    vector<Uint8> tab;

    while (i < dlugosc) {
        int znak = wejscie[i];
        if (znak >= 0) {
            for (int k = 0; k <= znak; k++) {
                tab.push_back(wejscie[i + k + 1] + 128);
            }
            i += znak + 2;
        } else {
            for (int k = 0; k <= abs(znak); k++) {
                tab.push_back(wejscie[i + 1] + 128);
            }
            i += 2;
        }
    }
    return tab;
}

// DCT
int normalizacja(vector<macierz> &blokDCTVector) {
    int maks = -999;
    for (auto &blokDCT : blokDCTVector) {
        for (int yy = 0; yy < rozmiarBloku; yy++) {
            for (int xx = 0; xx < rozmiarBloku; xx++) {
                // if(!(xx == 0 && yy == 0)){
                // cout<<"XX: "<<xx<<"YY: "<<yy<<endl;
                blokDCT.dct[xx][yy] = (round(blokDCT.dct[xx][yy] / dzielnik)) + 127;
                if (maks < blokDCT.dct[xx][yy]) maks = blokDCT.dct[xx][yy];
                //}
            }
        }
    }
    return maks;
}

macierz dct(Uint8 wartosci[rozmiarBloku][rozmiarBloku]) {
    float wynik[rozmiarBloku][rozmiarBloku];

    for (int v = 0; v < rozmiarBloku; ++v) {
        for (int u = 0; u < rozmiarBloku; ++u) {
            const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
            const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
            double wspolczynnikDCT = 0;

            for (int y = 0; y < rozmiarBloku; ++y) {
                for (int x = 0; x < rozmiarBloku; ++x) {
                    double uCosFactor = cos((double)(2 * x + 1) * M_PI * (double)u / (2 * (double)rozmiarBloku));
                    double vCosFactor = cos((double)(2 * y + 1) * M_PI * (double)v / (2 * (double)rozmiarBloku));
                    double pixel = (double)wartosci[x][y];
                    wspolczynnikDCT += pixel * uCosFactor * vCosFactor;
                }
            }
            wspolczynnikDCT *= (2.0 / (double)rozmiarBloku) * cu * cv;
            wynik[u][v] = round(wspolczynnikDCT);
        }
    }

    macierz rezultat;
    for (int j = 0; j < rozmiarBloku; j++) {
        for (int i = 0; i < rozmiarBloku; i++) {
            if ((i == 0 && j == 0) && abs(wynik[i][j]) > maksDCT) maksDCT = abs(wynik[i][j]);
            rezultat.dct[i][j] = wynik[i][j];
            rezultat.dane[i][j] = wartosci[i][j];
        }
    }

    return rezultat;
}

macierz idct(float DCT[rozmiarBloku][rozmiarBloku]) {
    int wynik[rozmiarBloku][rozmiarBloku];
    for (int x = 0; x < rozmiarBloku; ++x) {
        for (int y = 0; y < rozmiarBloku; ++y) {
            double pixel = 0;

            for (int u = 0; u < rozmiarBloku; ++u) {
                for (int v = 0; v < rozmiarBloku; ++v) {
                    const double cu = (u == 0) ? 1.0 / sqrt(2) : 1.0;
                    const double cv = (v == 0) ? 1.0 / sqrt(2) : 1.0;
                    double uCosFactor = cos((double)(2 * x + 1) * M_PI * (double)u / (2 * (double)rozmiarBloku));
                    double vCosFactor = cos((double)(2 * y + 1) * M_PI * (double)v / (2 * (double)rozmiarBloku));
                    double wspolczynnikDCT = DCT[u][v];
                    pixel += wspolczynnikDCT * uCosFactor * cu * vCosFactor * cv;
                }
            }
            pixel *= (2.0 / (double)rozmiarBloku);
            wynik[x][y] = round(pixel);
        }
    }

    macierz rezultat;

    for (int j = 0; j < rozmiarBloku; j++) {
        for (int i = 0; i < rozmiarBloku; i++) {
            if (wynik[i][j] > 255) wynik[i][j] = 255;
            if (wynik[i][j] < 0) wynik[i][j] = 0;
            rezultat.dane[i][j] = wynik[i][j];
            rezultat.dct[i][j] = DCT[i][j];
        }
    }
    return rezultat;
}
