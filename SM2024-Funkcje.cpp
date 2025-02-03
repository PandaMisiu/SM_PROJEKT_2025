#include <SDL2/SDL.h>
#include <conio.h>
#include <math.h>
#include <stdio.h>

#include <fstream>

#include "SM2024-Dithering.h"
#include "SM2024-Filtry.h"
#include "SM2024-Funkcje.h"
#include "SM2024-Kompresja.h"
#include "SM2024-Konwersje.h"
#include "SM2024-Pliki.h"
#include "SM2024-Zmienne.h"

// !!! WYBIERAMY OBRAZEK I KLIKAMY 1 !!!

// INICJALIZACJA
// TRYB:
// - kolor = 1
// - szary = 2
// KOMPRESJA:
// - ByteRun = 1
// - DCT = 2
void Funkcja1() {
    string wybranyTryb = "", wybranaPrzestrzen = "", wybranaKompresja = "";

    cout << "WYBIERZ TRYB KOLORU:\n";
    cout << "z - KOLOR\n";
    cout << "x - SZARY\n";

    char pickedChar;
    while (pickedChar = getch()) {
        if (pickedChar == 'z') {
            wybranyTryb = "KOLOR";
            break;
        } else if (pickedChar == 'x') {
            wybranyTryb = "SZARY";
            break;
        }
        cout << "WYBIERZ POPRAWNY ZNAK!\n";
    }

    system("cls");
    cout << "WYBIERZ PRZESTRZEN BARW:\n";
    cout << "z - RGB555\n";
    cout << "x - RGB888\n";
    cout << "c - YCbCr\n";

    pickedChar = NULL;
    while (pickedChar = getch()) {
        if (pickedChar == 'z') {
            wybranaPrzestrzen = "RGB555";
            break;
        } else if (pickedChar == 'x') {
            wybranaPrzestrzen = "RGB888";
            break;
        } else if (pickedChar == 'c') {
            wybranaPrzestrzen = "YCbCr";
            break;
        }
        cout << "WYBIERZ POPRAWNY ZNAK!\n";
    }

    system("cls");
    cout << "WYBIERZ RODZAJ KOMPRESJI:\n";
    cout << "z - BEZSTRATNA (ByteRun)\n";
    cout << "x - STRATNA (Transformata DCT)\n";

    pickedChar = NULL;
    while (pickedChar = getch()) {
        if (pickedChar == 'z') {
            wybranaKompresja = "BEZSTRATNA";
            break;
        } else if (pickedChar == 'x') {
            wybranaKompresja = "STRATNA";
            break;
        }
        cout << "WYBIERZ POPRAWNY ZNAK!\n";
    }

    bool isDithering;
    if (wybranaPrzestrzen == "RGB555") {
        system("cls");
        cout << "CZY CHCESZ UZYC DITHERING?:\n";
        cout << "z - TAK\n";
        cout << "x - NIE\n";

        pickedChar = NULL;
        while (pickedChar = getch()) {
            if (pickedChar == 'z') {
                isDithering = true;
                break;
            } else if (pickedChar == 'x') {
                isDithering = false;
                break;
            }
            cout << "WYBIERZ POPRAWNY ZNAK!\n";
        }
    }

    system("cls");
    cout << "WYBRANY TRYB:\t\t " << wybranyTryb << endl;
    cout << "WYBRANA PRZESTRZEN:\t " << wybranaPrzestrzen << endl;
    cout << "WYBRANA KOMPRESJA:\t " << wybranaKompresja << endl;

    cout << "!JESLI OBRAZEK NIE WYSWIETLI SIE, ZALADUJ PONOWNIE DOWOLNY OBRAZEK!\n";

    Uint8 tryb, kompresja;
    wybranyTryb == "KOLOR" ? tryb = 1 : tryb = 2;
    wybranaKompresja == "BEZSTRATNA" ? kompresja = 1 : kompresja = 2;

    cout << "TRYB I KOMPRESJA: " << (int)tryb << ", " << (int)kompresja << "\n";
    if (wybranaPrzestrzen == "RGB555") {
        RGB555ToFile(tryb, isDithering, kompresja);
        RGB555FromFile();
    } else if (wybranaPrzestrzen == "RGB888") {
        RGB888ToFile(tryb, kompresja);
        RGB888FromFile();
    } else {
        YCbCr888ToFile(tryb, kompresja);
        YCbCr888FromFile();
    }

    SDL_UpdateWindowSurface(window);
}

// TODO:

// RGB555:
// - predykcja
//
// RGB888:
// - predykcja
//
// YCbCr:
// - predykcja
// - subsampling?
//

// ------------- ||
// DO TESTOWANIA ||
// ------------- vv

// RGB555
void Funkcja2() {
    // TRYB:
    // - kolor = 1
    // - szary = 2
    // KOMPRESJA:
    // - ByteRun = 1
    // - DCT = 2
    Uint8 tryb = 1;
    Uint8 kompresja = 2;
    bool isDithering = true;

    RGB555ToFile(tryb, isDithering, kompresja);
    RGB555FromFile();

    SDL_UpdateWindowSurface(window);
}

// RGB888
void Funkcja3() {
    // TRYB:
    // - kolor = 1
    // - szary = 2
    // KOMPRESJA:
    // - ByteRun = 1
    // - DCT = 2

    Uint8 tryb = 1;
    Uint8 kompresja = 2;

    RGB888ToFile(tryb, kompresja);
    RGB888FromFile();

    SDL_UpdateWindowSurface(window);
}

// YCbCr
void Funkcja4() {
    // TRYB:
    // - kolor = 1
    // - szary = 2
    // KOMPRESJA:
    // - ByteRun = 1
    // - DCT = 2

    Uint8 tryb = 2;
    Uint8 kompresja = 2;

    YCbCr888ToFile(tryb, kompresja);
    YCbCr888FromFile();

    SDL_UpdateWindowSurface(window);
}
void Funkcja5() { SDL_UpdateWindowSurface(window); }

void Funkcja6() { SDL_UpdateWindowSurface(window); }

void Funkcja7() { SDL_UpdateWindowSurface(window); }

void Funkcja8() { SDL_UpdateWindowSurface(window); }

void Funkcja9() { SDL_UpdateWindowSurface(window); }

void Funkcja10() { SDL_UpdateWindowSurface(window); }

void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B) {
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc)) {
        /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informację ile bajtów zajmuje jeden piksel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres piksela */
        Uint8 *p1 = (Uint8 *)screen->pixels + (y * 2) * screen->pitch + (x * 2) * bpp;
        Uint8 *p2 = (Uint8 *)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2) * bpp;
        Uint8 *p3 = (Uint8 *)screen->pixels + (y * 2) * screen->pitch + (x * 2 + 1) * bpp;
        Uint8 *p4 = (Uint8 *)screen->pixels + (y * 2 + 1) * screen->pitch + (x * 2 + 1) * bpp;

        /* Ustawiamy wartość piksela, w zależnoœci od formatu powierzchni*/
        switch (bpp) {
            case 1:  // 8-bit
                *p1 = pixel;
                *p2 = pixel;
                *p3 = pixel;
                *p4 = pixel;
                break;

            case 2:  // 16-bit
                *(Uint16 *)p1 = pixel;
                *(Uint16 *)p2 = pixel;
                *(Uint16 *)p3 = pixel;
                *(Uint16 *)p4 = pixel;
                break;

            case 3:  // 24-bit
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    p1[0] = (pixel >> 16) & 0xff;
                    p1[1] = (pixel >> 8) & 0xff;
                    p1[2] = pixel & 0xff;
                    p2[0] = (pixel >> 16) & 0xff;
                    p2[1] = (pixel >> 8) & 0xff;
                    p2[2] = pixel & 0xff;
                    p3[0] = (pixel >> 16) & 0xff;
                    p3[1] = (pixel >> 8) & 0xff;
                    p3[2] = pixel & 0xff;
                    p4[0] = (pixel >> 16) & 0xff;
                    p4[1] = (pixel >> 8) & 0xff;
                    p4[2] = pixel & 0xff;
                } else {
                    p1[0] = pixel & 0xff;
                    p1[1] = (pixel >> 8) & 0xff;
                    p1[2] = (pixel >> 16) & 0xff;
                    p2[0] = pixel & 0xff;
                    p2[1] = (pixel >> 8) & 0xff;
                    p2[2] = (pixel >> 16) & 0xff;
                    p3[0] = pixel & 0xff;
                    p3[1] = (pixel >> 8) & 0xff;
                    p3[2] = (pixel >> 16) & 0xff;
                    p4[0] = pixel & 0xff;
                    p4[1] = (pixel >> 8) & 0xff;
                    p4[2] = (pixel >> 16) & 0xff;
                }
                break;

            case 4:  // 32-bit
                *(Uint32 *)p1 = pixel;
                *(Uint32 *)p2 = pixel;
                *(Uint32 *)p3 = pixel;
                *(Uint32 *)p4 = pixel;
                break;
        }
    }
}

void setPixelSurface(int x, int y, Uint8 R, Uint8 G, Uint8 B) {
    if ((x >= 0) && (x < szerokosc * 2) && (y >= 0) && (y < wysokosc * 2)) {
        /* Zamieniamy poszczególne składowe koloru na format koloru piksela */
        Uint32 pixel = SDL_MapRGB(screen->format, R, G, B);

        /* Pobieramy informację ile bajtów zajmuje jeden piksel */
        int bpp = screen->format->BytesPerPixel;

        /* Obliczamy adres piksela */
        Uint8 *p = (Uint8 *)screen->pixels + y * screen->pitch + x * bpp;

        /* Ustawiamy wartość piksela, w zależności od formatu powierzchni*/
        switch (bpp) {
            case 1:  // 8-bit
                *p = pixel;
                break;

            case 2:  // 16-bit
                *(Uint16 *)p = pixel;
                break;

            case 3:  // 24-bit
                if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                } else {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;

            case 4:  // 32-bit
                *(Uint32 *)p = pixel;
                break;
        }
    }
}

SDL_Color getPixel(int x, int y) {
    SDL_Color color;
    Uint32 col = 0;
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc)) {
        // określamy pozycję
        char *pPosition = (char *)screen->pixels;

        // przesunięcie względem y
        pPosition += (screen->pitch * y * 2);

        // przesunięcie względem x
        pPosition += (screen->format->BytesPerPixel * x * 2);

        // kopiujemy dane piksela
        memcpy(&col, pPosition, screen->format->BytesPerPixel);

        // konwertujemy kolor
        SDL_GetRGB(col, screen->format, &color.r, &color.g, &color.b);
    }
    return (color);
}

SDL_Color getPixelSurface(int x, int y, SDL_Surface *surface) {
    SDL_Color color;
    Uint32 col = 0;
    if ((x >= 0) && (x < szerokosc) && (y >= 0) && (y < wysokosc)) {
        // określamy pozycję
        char *pPosition = (char *)surface->pixels;

        // przesunięcie względem y
        pPosition += (surface->pitch * y);

        // przesunięcie względem x
        pPosition += (surface->format->BytesPerPixel * x);

        // kopiujemy dane piksela
        memcpy(&col, pPosition, surface->format->BytesPerPixel);

        // konwertujemy kolor
        SDL_GetRGB(col, surface->format, &color.r, &color.g, &color.b);
    }
    return (color);
}

void ladujBMP(char const *nazwa, int x, int y) {
    SDL_Surface *bmp = SDL_LoadBMP(nazwa);
    if (!bmp) {
        printf("Unable to load bitmap: %s\n", SDL_GetError());
    } else {
        SDL_Color kolor;
        for (int yy = 0; yy < bmp->h; yy++) {
            for (int xx = 0; xx < bmp->w; xx++) {
                kolor = getPixelSurface(xx, yy, bmp);
                setPixel(xx, yy, kolor.r, kolor.g, kolor.b);
            }
        }
        SDL_FreeSurface(bmp);
        SDL_UpdateWindowSurface(window);
    }
}

void czyscEkran(Uint8 R, Uint8 G, Uint8 B) {
    SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, R, G, B));
    SDL_UpdateWindowSurface(window);
}
