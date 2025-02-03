#include "SM2024-Pliki.h"

#include <stdio.h>
#include <math.h>
#include <fstream>
#include <SDL2/SDL.h>

#define szerokosc 640
#define wysokosc 400

#include "SM2024-Funkcje.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Konwersje.h"
#include "SM2024-Dithering.h"
#include "SM2024-Kompresja.h"


std::ofstream zapiszDoPliku(Uint16 szerokoscObrazka, Uint16 wysokoscObrazka, Uint8 tryb, Uint8 dithering, Uint8 kompresja){
    char identyfikator[] = "13";
    string plik;
    cout << "Podaj nazwe pliku do ktorego zapisany zostanie obraz: ";
    cin >> plik;
    ofstream wyjscie(plik, ios::binary);
    wyjscie.write((char*)&identyfikator, sizeof(char) * 2);
    wyjscie.write((char*)&szerokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&wysokoscObrazka, sizeof(Uint16));
    wyjscie.write((char*)&tryb, sizeof(Uint8));
    wyjscie.write((char*)&dithering, sizeof(Uint8));
    wyjscie.write((char*)&kompresja, sizeof(Uint8));

    return wyjscie;
}


std::ifstream odczytPliku(Uint16 &szerokoscObrazka, Uint16 &wysokoscObrazka, Uint8 &tryb, Uint8 &dithering, Uint8 &kompresja){
    char identyfikator[] = "13";
    cout<<"Podaj nazwe pliku do odczytu: ";

    string plik;
    cin >> plik;

    cout << "odczytujemy plik " + plik +" uzywajac metody read()" << endl;
    ifstream wejscie(plik, ios::binary);

    wejscie.read((char*)&identyfikator, sizeof(char) * 2);
    wejscie.read((char*)&szerokoscObrazka, sizeof(Uint16));
    wejscie.read((char*)&wysokoscObrazka, sizeof(Uint16));
    wejscie.read((char*)&tryb, sizeof(Uint8));
    wejscie.read((char*)&dithering, sizeof(Uint8));
    wejscie.read((char*)&kompresja, sizeof(Uint8));

    cout << "id: " << identyfikator << endl;
    cout << "szerokosc: " << szerokoscObrazka << endl;
    cout << "wysokosc: " << wysokoscObrazka << endl;
    cout << "tryb: " << (int)tryb << endl;
    cout << "dithering: " << (int)dithering << endl;
    cout << "kompresja: " << (int)kompresja << endl;


    return wejscie;
}

void saveToFileByteRun_COLOR(std::ofstream& wyjscie, Uint8 A_tab[], Uint8 B_tab[], Uint8 C_tab[], int dlugosc){
    vector<Sint8> A_compressed = byteRunKompresja(A_tab,dlugosc);
    vector<Sint8> B_compressed = byteRunKompresja(B_tab,dlugosc);
    vector<Sint8> C_compressed = byteRunKompresja(C_tab,dlugosc);

    Sint32 A_tabSize = A_compressed.size();
    Sint32 B_tabSize = B_compressed.size();
    Sint32 C_tabSize = C_compressed.size();

    wyjscie.write((char*)&A_tabSize, sizeof(Sint32));
    wyjscie.write((char*)&B_tabSize, sizeof(Sint32));
    wyjscie.write((char*)&C_tabSize, sizeof(Sint32));

    for (int i = 0; i < A_tabSize; i++) {
        wyjscie.write((char*)&A_compressed[i], sizeof(Sint8));
    }

    for (int i = 0; i < B_tabSize; i++) {
        wyjscie.write((char*)&B_compressed[i], sizeof(Sint8));
    }

    for (int i = 0; i < C_tabSize; i++) {
        wyjscie.write((char*)&C_compressed[i], sizeof(Sint8));
    }

    wyjscie.close();
}

void saveToFileByteRun_GRAY(std::ofstream& wyjscie, Uint8 GRAY_tab[], int dlugosc) {
    vector<Sint8> GRAY_compressed = byteRunKompresja(GRAY_tab,dlugosc);

    Sint32 GRAY_tabSize = GRAY_compressed.size();

    wyjscie.write((char*)&GRAY_tabSize, sizeof(Sint32));

    for (int i = 0; i < GRAY_tabSize; i++) {
        wyjscie.write((char*)&GRAY_compressed[i], sizeof(Sint8));
    }

    wyjscie.close();
}

// [POMOCNICZA]
void saveDCTBlocks(std::ofstream& wyjscie, std::vector<macierz> blokDCTVector){
    for(auto blokDCT : blokDCTVector){
        for (int diag = 0; diag < 2 * rozmiarBloku - 1; ++diag) {
        const auto i_min = std::max(0, diag - rozmiarBloku + 1);
        const auto i_max = i_min + std::min(diag, 2 * (rozmiarBloku - 1) - diag);
            for (auto i = i_min; i <= i_max; ++i) {
                const auto row = diag % 2 ? i : (diag - i);
                const auto col = diag % 2 ? (diag - i) : i;
                Uint8 byte = (Uint8)blokDCT.dct[row][col];
                wyjscie.write((char*)&byte,sizeof(Uint8));
            }
        }
    }
}

// [POMOCNICZA]
void findMaxDivider(){
    while(((maksDCT/dzielnik)+127)>255) dzielnik++;
}

void saveToFileDCT_COLOR(std::ofstream& wyjscie,ColorTypes colorType){
    macierz blokDCT_A,blokDCT_B,blokDCT_C;
    macierz blokDane_A,blokDane_B,blokDane_C;
    macierz noweDane_A,noweDane_B,noweDane_C;
    vector<macierz> blokDCTVector_A, blokDCTVector_B, blokDCTVector_C;

    // ZBIERANIE DANYCH
    SDL_Color color;
    for(int y=0;y<wysokosc/2;y+=rozmiarBloku){
        for(int x=0;x<szerokosc/2;x+=rozmiarBloku){
            for (int yy=0; yy<rozmiarBloku; yy++) {
                for (int xx=0; xx<rozmiarBloku; xx++) {
                    if(colorType == RGB888_TYPE){
                        color = getPixel(xx+x,yy+y);
                        blokDane_A.dane[xx][yy] = color.r;
                        blokDane_B.dane[xx][yy] = color.g;
                        blokDane_C.dane[xx][yy] = color.b;
                    }
                    else if(colorType == YCbCr_TYPE){
                        color = getPixel(xx+x,yy+y);
                        YCbCr ycbcr = RGBToYCbCr(color);
                        blokDane_A.dane[xx][yy] = ycbcr.y;
                        blokDane_B.dane[xx][yy] = ycbcr.cb;
                        blokDane_C.dane[xx][yy] = ycbcr.cr;
                    }
                    else if(colorType == RGB555_TYPE){
                        color = getRGB555(xx+x,yy+y);
                        blokDane_A.dane[xx][yy] = color.r;
                        blokDane_B.dane[xx][yy] = color.g;
                        blokDane_C.dane[xx][yy] = color.b;
                    }

                    blokDane_A.dct[xx][yy] = 0;
                    blokDane_B.dct[xx][yy] = 0;
                    blokDane_C.dct[xx][yy] = 0;
                }
            }
            blokDCT_A = dct(blokDane_A.dane);
            blokDCTVector_A.push_back(blokDCT_A);

            blokDCT_B = dct(blokDane_B.dane);
            blokDCTVector_B.push_back(blokDCT_B);

            blokDCT_C = dct(blokDane_C.dane);
            blokDCTVector_C.push_back(blokDCT_C);
        }
    }

    // SZUKANIE DZIELNIKA
    findMaxDivider();

    // NORMALIZACJA
    normalizacja(blokDCTVector_A);
    normalizacja(blokDCTVector_B);
    normalizacja(blokDCTVector_C);

    // ZAPIS DO PLIKU
    wyjscie.write((char*)&dzielnik, sizeof(Uint16));
    saveDCTBlocks(wyjscie, blokDCTVector_A);
    saveDCTBlocks(wyjscie, blokDCTVector_B);
    saveDCTBlocks(wyjscie, blokDCTVector_C);

    wyjscie.close();
}

void saveToFileDCT_GRAY(std::ofstream& wyjscie,ColorTypes colorType){
    macierz blokDCT;
    macierz blokDane;
    macierz noweDane;
    vector<macierz> blokDCTVector;

    // ZBIERANIE DANYCH
    SDL_Color color;
    for(int y=0;y<wysokosc/2;y+=rozmiarBloku){
        for(int x=0;x<szerokosc/2;x+=rozmiarBloku){
            for (int yy=0; yy<rozmiarBloku; yy++) {
                for (int xx=0; xx<rozmiarBloku; xx++) {

                    if(colorType == RGB888_TYPE){
                        color = getPixel(xx+x,yy+y);
                        Uint8 szary = konwertujNaSzary(color);
                        blokDane.dane[xx][yy] = szary;
                    }
                    else if(colorType == YCbCr_TYPE){
                        color = getPixel(xx+x,yy+y);
                        YCbCr ycbcr = RGBToYCbCr(color);
                        blokDane.dane[xx][yy] = ycbcr.y;
                    }
                    else if(colorType == RGB555_TYPE){
                        color = getRGB555(xx+x,yy+y);
                        Uint8 szary = konwertujNaSzary(color);
                        blokDane.dane[xx][yy] = szary;
                    }

                    blokDane.dct[xx][yy] = 0;
                }
            }
            blokDCT = dct(blokDane.dane);
            blokDCTVector.push_back(blokDCT);
        }
    }

    // SZUKANIE DZIELNIKA
    findMaxDivider();

    // NORMALIZACJA
    normalizacja(blokDCTVector);

    // ZAPIS DO PLIKU
    wyjscie.write((char*)&dzielnik, sizeof(Uint16));
    saveDCTBlocks(wyjscie,blokDCTVector);

    wyjscie.close();
}

void readFromFileByteRun_COLOR(std::ifstream& wejscie, ColorTypes colorType, bool isDithering) {
    vector<Sint8> A_compressed, B_compressed, C_compressed;
    vector<Uint8> A_tab, B_tab, C_tab;
    Sint8 byte;

    Sint32 A_tabSize, B_tabSize, C_tabSize;
    wejscie.read((char*)&A_tabSize, sizeof(Sint32));
    wejscie.read((char*)&B_tabSize, sizeof(Sint32));
    wejscie.read((char*)&C_tabSize, sizeof(Sint32));

    for(int i=0;i<A_tabSize;i++){
        wejscie.read((char*)&byte, sizeof(Sint8));
        A_compressed.push_back(byte);
    }

    for(int i=0;i<B_tabSize;i++){
        wejscie.read((char*)&byte, sizeof(Sint8));
        B_compressed.push_back(byte);
    }

    for(int i=0;i<C_tabSize;i++){
        wejscie.read((char*)&byte, sizeof(Sint8));
        C_compressed.push_back(byte);
    }

    A_tab = byteRunDekompresja(A_compressed,A_tabSize);
    B_tab = byteRunDekompresja(B_compressed,B_tabSize);
    C_tab = byteRunDekompresja(C_compressed,C_tabSize);

    // DITHERING
    int rozmiar = 2;
    Uint8 tablicaD;
    if(isDithering){
        zaktualizujTabliceBayera2();
    }

    // WYSWIETALNIE
    int index = 0;
    for (int y = 0; y < wysokosc/2; y++) {
        for (int x = 0; x < szerokosc/2; x++) {
            if(colorType == RGB888_TYPE){
                setPixel(x + szerokosc/2, y, A_tab[index], B_tab[index], C_tab[index]);
            }
            else if(colorType == YCbCr_TYPE){
                YCbCr ycbcr = YCbCr(A_tab[index],B_tab[index],C_tab[index]);
                SDL_Color rgb = YCbCrToRGB(ycbcr);
                setPixel(x + szerokosc/2, y, rgb.r,rgb.g, rgb.b);
            }
            else if(colorType == RGB555_TYPE){
                Uint8 R = A_tab[index];
                Uint8 G = B_tab[index];
                Uint8 B = C_tab[index];

                if(isDithering) {
                    tablicaD = zaktualizowanaTablicaBayera2 [y % rozmiar] [x % rozmiar];

                    R = R>tablicaD ? 31 : 0;
                    G = G>tablicaD ? 31 : 0;
                    B = B>tablicaD ? 31 : 0;
                }

                setRGB555(x + szerokosc/2, y, R,G,B);
            }

            index++;
        }
    }

    SDL_UpdateWindowSurface(window);
    wejscie.close();
}

void readFromFileByteRun_GRAY(std::ifstream& wejscie, ColorTypes colorType, bool isDithering){
    vector<Sint8> GRAY_compressed;
    vector<Uint8> GRAY_tab;
    Sint8 byte;

    Sint32 GRAY_tabSize;
    wejscie.read((char*)&GRAY_tabSize, sizeof(Sint32));

    for(int i=0;i<GRAY_tabSize;i++){
        wejscie.read((char*)&byte, sizeof(Sint8));
        GRAY_compressed.push_back(byte);
    }

    GRAY_tab = byteRunDekompresja(GRAY_compressed,GRAY_tabSize);

    // DITHERING
    int rozmiar = 2;
    Uint8 tablicaGRAY;
    if(isDithering){
        zaktualizujTabliceBayera2();
    }

    // WYSWIETLANIE
    int index = 0;
    for (int y = 0; y < wysokosc/2; y++) {
        for (int x = 0; x < szerokosc/2; x++) {
            if(colorType == RGB555_TYPE) {
                Uint8 GRAY = GRAY_tab[index];

                if(isDithering) {
                    tablicaGRAY = zaktualizowanaTablicaBayera2 [y % rozmiar] [x % rozmiar];
                    GRAY = GRAY>tablicaGRAY ? 31 : 0;
                }
                setRGB555(x + szerokosc/2, y, GRAY,GRAY,GRAY);
            }
            else setPixel(x + szerokosc/2, y, GRAY_tab[index], GRAY_tab[index], GRAY_tab[index]);

            index++;
        }
    }

    SDL_UpdateWindowSurface(window);
    wejscie.close();
}

// [POMOCNICZA]
std::vector<macierz> readDCTBlocks(std::ifstream& wejscie,int liczbaBlokow, int odczytanyDzielnik){
    vector<macierz> blokDCTVector;

    for (int blok = 0; blok < liczbaBlokow; blok++) {
        macierz blokDCT;
        for (int diag = 0; diag < 2 * rozmiarBloku - 1; ++diag) {
            const auto i_min = std::max(0, diag - rozmiarBloku + 1);
            const auto i_max = i_min + std::min(diag, 2 * (rozmiarBloku - 1) - diag);
            for (auto i = i_min; i <= i_max; ++i) {
                const auto row = diag % 2 ? i : (diag - i);
                const auto col = diag % 2 ? (diag - i) : i;
                Uint8 value;
                wejscie.read((char*)&value, sizeof(Uint8));
                if (!wejscie) break;
                blokDCT.dct[row][col] = (value - 127) * odczytanyDzielnik;
            }
        }
        blokDCTVector.push_back(blokDCT);
    }
    return blokDCTVector;
}

void readFromFileDCT_COLOR(std::ifstream& wejscie, ColorTypes colorType, bool isDithering){
    Uint16 odczytanyDzielnik;
    wejscie.read((char *)&odczytanyDzielnik, sizeof(Uint16));

    int liczbaBlokowX = szerokosc/2 / rozmiarBloku;
    int liczbaBlokowY = wysokosc/2 / rozmiarBloku;
    int liczbaBlokow = liczbaBlokowX * liczbaBlokowY;

    // ODCZYTANIE BLOKOW
    vector<macierz> blokDCTVector_A = readDCTBlocks(wejscie,liczbaBlokow, odczytanyDzielnik);
    vector<macierz> blokDCTVector_B = readDCTBlocks(wejscie,liczbaBlokow, odczytanyDzielnik);
    vector<macierz> blokDCTVector_C = readDCTBlocks(wejscie,liczbaBlokow, odczytanyDzielnik);

    // DITHERING
    int rozmiar = 2;
    Uint8 tablicaD;
    if(isDithering){
        zaktualizujTabliceBayera2();
    }

    // WYSWIETLANIE
    int index = 0;
    for (int by = 0; by < liczbaBlokowY; by++) {
        for (int bx = 0; bx < liczbaBlokowX; bx++) {

            macierz blokDCT_A = blokDCTVector_A[index];
            macierz blokDCT_B = blokDCTVector_B[index];
            macierz blokDCT_C = blokDCTVector_C[index];
            index++;

            macierz odtworzonyBlok_A = idct(blokDCT_A.dct);
            macierz odtworzonyBlok_B = idct(blokDCT_B.dct);
            macierz odtworzonyBlok_C = idct(blokDCT_C.dct);

            for (int yy = 0; yy < rozmiarBloku; yy++) {
                for (int xx = 0; xx < rozmiarBloku; xx++) {
                    SDL_Color rgb;
                    if(colorType==RGB888_TYPE){
                        rgb = {odtworzonyBlok_A.dane[xx][yy],odtworzonyBlok_B.dane[xx][yy],odtworzonyBlok_C.dane[xx][yy]};
                        setPixel(bx * rozmiarBloku + xx+szerokosc/2,by * rozmiarBloku + yy,rgb.r, rgb.g, rgb.b);
                    }
                    else if(colorType==YCbCr_TYPE) {
                        YCbCr ycbcr = YCbCr(odtworzonyBlok_A.dane[xx][yy],odtworzonyBlok_B.dane[xx][yy],odtworzonyBlok_C.dane[xx][yy]);
                        rgb = YCbCrToRGB(ycbcr);
                        setPixel(bx * rozmiarBloku + xx+szerokosc/2,by * rozmiarBloku + yy,rgb.r, rgb.g, rgb.b);
                    }

                    else if(colorType == RGB555_TYPE){
                        Uint8 R = odtworzonyBlok_A.dane[xx][yy];
                        Uint8 G = odtworzonyBlok_B.dane[xx][yy];
                        Uint8 B = odtworzonyBlok_C.dane[xx][yy];
                        if(isDithering) {
                            tablicaD = zaktualizowanaTablicaBayera2 [yy % rozmiar] [xx % rozmiar];

                            R = R>tablicaD ? 31 : 0;
                            G = G>tablicaD ? 31 : 0;
                            B = B>tablicaD ? 31 : 0;
                        }

                        setRGB555(bx * rozmiarBloku + xx+szerokosc/2,by * rozmiarBloku + yy,R,G,B);
                    }
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
    wejscie.close();
}

void readFromFileDCT_GRAY(std::ifstream& wejscie, ColorTypes colorType, bool isDithering){
    Uint16 odczytanyDzielnik;
    wejscie.read((char*)&odczytanyDzielnik, sizeof(Uint16));

    int liczbaBlokowX = szerokosc/2 / rozmiarBloku;
    int liczbaBlokowY = wysokosc/2 / rozmiarBloku;
    int liczbaBlokow = liczbaBlokowX * liczbaBlokowY;

    // DITHERING
    int rozmiar = 2;
    Uint8 tablicaGRAY;
    if(isDithering){
        zaktualizujTabliceBayera2();
    }


    // ODCZYTANIE BLOKOW
    vector<macierz> blokDCTVector = readDCTBlocks(wejscie,liczbaBlokow,odczytanyDzielnik);

    // WYSWIETLENIE
    int index = 0;
    for (int by = 0; by < liczbaBlokowY; by++) {
        for (int bx = 0; bx < liczbaBlokowX; bx++) {

            macierz blokDCT = blokDCTVector[index];
            macierz odtworzonyBlok = idct(blokDCT.dct);
            index++;

            for (int yy = 0; yy < rozmiarBloku; yy++) {
                for (int xx = 0; xx < rozmiarBloku; xx++) {
                    Uint8 GRAY = odtworzonyBlok.dane[xx][yy];

                    if(colorType == RGB555_TYPE){
                        if(isDithering) {
                            tablicaGRAY = zaktualizowanaTablicaBayera2 [yy % rozmiar] [xx % rozmiar];
                            GRAY = GRAY>tablicaGRAY ? 31 : 0;
                        }

                        setRGB555(
                            bx * rozmiarBloku + xx+szerokosc/2,
                            by * rozmiarBloku + yy,
                            GRAY,GRAY,GRAY
                        );
                    }
                    else {
                        setPixel(
                            bx * rozmiarBloku + xx+szerokosc/2,
                            by * rozmiarBloku + yy,
                            GRAY,GRAY,GRAY
                        );
                    }
                }
            }
        }
    }

    SDL_UpdateWindowSurface(window);
    wejscie.close();
}

// RGB888
void RGB888ToFile(Uint8 pickedTryb, Uint8 pickedKompresja){
    Uint16 szerokoscObrazka = szerokosc / 2;
    Uint16 wysokoscObrazka = wysokosc / 2;
    Uint8 tryb = pickedTryb;
    Uint8 dithering = 0;
    Uint8 kompresja = pickedKompresja;

    cout << "[RGB888]" << endl;
    std::ofstream wyjscie;

    if((int)kompresja==1) { // BYTERUN
        int dlugosc = szerokoscObrazka*wysokoscObrazka;

        Uint8 R_tab[dlugosc];
        Uint8 G_tab[dlugosc];
        Uint8 B_tab[dlugosc];

        int index = 0;
        for (int y = 0; y < wysokoscObrazka; y ++) {
            for (int x = 0; x < szerokoscObrazka; x ++) {
                SDL_Color rgb = getPixel(x,y);

                if((int)tryb == 1) { // KOLOR
                    R_tab[index] = rgb.r;
                    G_tab[index] = rgb.g;
                    B_tab[index] = rgb.b;
                }
                else {               // SZARY
                    Uint8 szary = konwertujNaSzary(rgb);
                    R_tab[index] = szary;
                }
                index++;
            }
        }

        wyjscie = zapiszDoPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering, kompresja);

        // ZAPIS BYTERUN
        if((int)tryb == 1) {
            saveToFileByteRun_COLOR(wyjscie,R_tab,G_tab,B_tab,dlugosc);
            return;
        }
        else {
            saveToFileByteRun_GRAY(wyjscie,R_tab,dlugosc);
            return;
        }
    }
    else { //DCT
        wyjscie = zapiszDoPliku(szerokoscObrazka,wysokoscObrazka,tryb,dithering,kompresja);

        // ZAPIS DCT
        if((int)tryb == 1) {
            saveToFileDCT_COLOR(wyjscie,RGB888_TYPE);
            return;
        }
        else {
            saveToFileDCT_GRAY(wyjscie,RGB888_TYPE);
            return;
        }
    }

    SDL_UpdateWindowSurface(window);
}

void RGB888FromFile() {
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    Uint8 tryb = 0;
    Uint8 dithering = 0;
    Uint8 kompresja = 0;

    std::ifstream wejscie = odczytPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering, kompresja);

    if((int)kompresja == 1) { // BYTERUN
        if((int)tryb == 1){
    readFromFileByteRun_COLOR(wejscie,RGB888_TYPE,false);
    return;
}
        else { // SZARY
            readFromFileByteRun_GRAY(wejscie,RGB888_TYPE,false);
            return;
        }
    }
    else { // DCT
        if((int)tryb==1){ // KOLOR
            readFromFileDCT_COLOR(wejscie,RGB888_TYPE,false);
            return;
        }
        else { // SZARY
            readFromFileDCT_GRAY(wejscie,RGB888_TYPE,false);
            return;
        }
    }

    SDL_UpdateWindowSurface(window);
}


void YCbCr888ToFile(Uint8 pickedTryb, Uint8 pickedKompresja){
    Uint16 szerokoscObrazka = szerokosc / 2;
    Uint16 wysokoscObrazka = wysokosc / 2;
    Uint8 tryb = pickedTryb;
    Uint8 dithering = 0;
    Uint8 kompresja = pickedKompresja;

    cout << "[YCbCr]" << endl;
    std::ofstream wyjscie;

    if((int)kompresja==1){
        int dlugosc = szerokoscObrazka*wysokoscObrazka;

        Uint8 Y_tab[dlugosc];
        Uint8 Cb_tab[dlugosc];
        Uint8 Cr_tab[dlugosc];

        int index = 0;
        for (int y = 0; y < wysokoscObrazka; y++) {
            for (int x = 0; x < szerokoscObrazka; x++) {
                SDL_Color rgb = getPixel(x, y);
                YCbCr ycbcr = RGBToYCbCr(rgb);

                if((int)tryb == 1){ // KOLOR
                    Y_tab[index] = ycbcr.y;
                    Cb_tab[index] = ycbcr.cb;
                    Cr_tab[index] = ycbcr.cr;
                }
                else { // SZARY
                    Y_tab[index] = ycbcr.y;
                }
                index++;
            }
        }

        wyjscie = zapiszDoPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering, kompresja);

        // ZAPIS BYTERUN
        if((int)tryb == 1) {
            saveToFileByteRun_COLOR(wyjscie,Y_tab,Cb_tab,Cr_tab,dlugosc);
            return;
        }
        else {
            saveToFileByteRun_GRAY(wyjscie,Y_tab,dlugosc);
            return;
        }
    }
    else { // DCT
        wyjscie = zapiszDoPliku(szerokoscObrazka,wysokoscObrazka,tryb,dithering,kompresja);

        if((int)tryb==1){ // KOLOR
            saveToFileDCT_COLOR(wyjscie,YCbCr_TYPE);
        }
        else { // SZARY
            saveToFileDCT_GRAY(wyjscie,YCbCr_TYPE);
        }
    }

    SDL_UpdateWindowSurface(window);
}

void YCbCr888FromFile(){
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    Uint8 tryb = 0;
    Uint8 dithering = 0;
    Uint8 kompresja = 0;

    std::ifstream wejscie = odczytPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering,kompresja);

    if((int)kompresja == 1){
        if((int)tryb == 1){
            readFromFileByteRun_COLOR(wejscie, YCbCr_TYPE,false);
            return;
        }
        else { // SZARY
            readFromFileByteRun_GRAY(wejscie, YCbCr_TYPE,false);
            return;
        }
    }
    else {
        if((int)tryb==1){ // KOLOR
            readFromFileDCT_COLOR(wejscie,YCbCr_TYPE,false);
            return;
        }
        else { // SZARY
            readFromFileDCT_GRAY(wejscie,YCbCr_TYPE,false);
            return;
        }
    }

    SDL_UpdateWindowSurface(window);
}

void RGB555ToFile(Uint8 pickedTryb,bool isDithering, Uint8 pickedKompresja) {
    Uint16 szerokoscObrazka = szerokosc / 2;
    Uint16 wysokoscObrazka = wysokosc / 2;
    Uint8 tryb = pickedTryb;
    Uint8 dithering = (Uint8)isDithering;
    Uint8 kompresja = pickedKompresja;

    cout << "[RGB555]" << endl;
    std::ofstream wyjscie;

    SDL_Color kolor;
    Uint8 R,G,B;
    int dlugosc = szerokoscObrazka*wysokoscObrazka;

    Uint8 R_tab[dlugosc];
    Uint8 G_tab[dlugosc];
    Uint8 B_tab[dlugosc];

    int index = 0;
    for (int y=0; y<wysokosc/2; y++) {
        for (int x=0; x<szerokosc/2; x++) {
            kolor = getRGB555(x, y);
            if((int)tryb==1) { // KOLOR
                R=kolor.r;
                G=kolor.g;
                B=kolor.b;
            }
            else { // SZARY
                Uint8 szary = konwertujNaSzary(kolor);
                R=szary;
                G=szary;
                B=szary;
            }

            R_tab[index] = R;
            G_tab[index] = G;
            B_tab[index] = B;

            index++;
        }
    }

    if((int)kompresja==1){ // BYTERUN
        wyjscie = zapiszDoPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering, kompresja);

        // ZAPIS BYTERUN
        if((int)tryb == 1) {
            saveToFileByteRun_COLOR(wyjscie,R_tab,G_tab,B_tab,dlugosc);
            return;
        }
        else {
            saveToFileByteRun_GRAY(wyjscie,R_tab,dlugosc);
            return;
        }
    }
    else { // DCT
        wyjscie = zapiszDoPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering, kompresja);

        if((int)tryb==1){ // KOLOR
            saveToFileDCT_COLOR(wyjscie,RGB555_TYPE);
            return;
        }
        else { // SZARY
            saveToFileDCT_GRAY(wyjscie, RGB555_TYPE);
            return;
        }
    }

    SDL_UpdateWindowSurface(window);
}

void RGB555FromFile() {
    Uint16 szerokoscObrazka = 0;
    Uint16 wysokoscObrazka = 0;
    Uint8 tryb = 0;
    Uint8 dithering = 0;
    Uint8 kompresja = 0;

    std::ifstream wejscie = odczytPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering, kompresja);

    Uint16 czytanyKolor;
    Uint16 rgb555_tab[szerokosc / 2][wysokosc / 2];

    if((int)kompresja == 1){
        if((int)tryb == 1){
            readFromFileByteRun_COLOR(wejscie, RGB555_TYPE,dithering);
            return;
        }
        else { // SZARY
            readFromFileByteRun_GRAY(wejscie, RGB555_TYPE,dithering);
            return;
        }
    }
    else { // DCT
        if((int)tryb==1){ // KOLOR
            readFromFileDCT_COLOR(wejscie,RGB555_TYPE,dithering);
        }
        else { // SZARY
            readFromFileDCT_GRAY(wejscie,RGB555_TYPE,dithering);
        }
    }

    SDL_UpdateWindowSurface(window);
}

