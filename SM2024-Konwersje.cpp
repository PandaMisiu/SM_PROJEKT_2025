#include "SM2024-Konwersje.h"

#include <stdio.h>
#include <math.h>
#include <fstream>
#include <SDL2/SDL.h>
#include "SM2024-Funkcje.h"
#include "SM2024-Zmienne.h"
#include "SM2024-Pliki.h"
#include "SM2024-Dithering.h"
#include "SM2024-Filtry.h"

float normalizacja(float num, int v_min, int v_max){
    if(num<v_min) num = v_min;
    else if(num>v_max) num = v_max;
    return num;
}

YCbCr RGBToYCbCr (SDL_Color rgb){

    float y = (0.299*(int)rgb.r)+(0.587*(int)rgb.g)+(0.114*(int)rgb.b);
    float cb = 128-(0.168736*(int)rgb.r)-(0.331264*(int)rgb.g)+(0.5*(int)rgb.b);
    float cr = 128+(0.5*(int)rgb.r)-(0.418688*(int)rgb.g)-(0.081312*(int)rgb.b);

    y = normalizacja(y,0,255);
    cb = normalizacja(cb,0,255);
    cr = normalizacja(cr,0,255);

    YCbCr ycbcr(y,cb,cr);

    return ycbcr;
}

SDL_Color YCbCrToRGB(YCbCr ycbcr){
    SDL_Color rgb;
    int R,G,B;

    R = ycbcr.y+1.402000 *(ycbcr.cr-128);
    G = ycbcr.y-0.344136 *(ycbcr.cb-128)-0.714136*(ycbcr.cr-128);
    B = ycbcr.y+1.772000 *(ycbcr.cb-128);

    rgb.r = normalizacja(R,0,255);
    rgb.g = normalizacja(G,0,255);
    rgb.b = normalizacja(B,0,255);

    return rgb;
}


SDL_Color getRGB555(int xx, int yy) {
    SDL_Color rgb = getPixel(xx,yy);

    Uint8 r, g, b;

    r = (rgb.r >> 3);
    g = (rgb.g >> 3);
    b = (rgb.b >> 3);

    return {r,g,b};
}

Uint16 getRGB555_(int xx, int yy) {
    SDL_Color rgb = getPixel(xx, yy);

    Uint8 r, g, b;

    r = (rgb.r >> 3);
    g = (rgb.g >> 3);
    b = (rgb.b >> 3);

    return ((r<<10)+(g<<5)+b);
}


void setRGB555(int xx,int yy,Uint8 r,Uint8 g,Uint8 b){
    Uint8 R,G,B;

    R = r<<3;
    G = g<<3;
    B = b<<3;

    setPixel(xx,yy,R,G,B);
}

void setRGB555_(int xx,int yy,Uint16 rgb555){
    Uint8 R,G,B;

    R = (rgb555 >> 10) & 0x1F;
    R = R << 3;

    G = (rgb555 >> 5) & 0x1F;
    G = G << 3;

    B = rgb555 & 0x1F;
    B = B << 3;

    setPixel(xx,yy,R,G,B);
}

void YCbCrSubSampling() {
    Uint16 szerokoscObrazka = szerokosc / 2;
    Uint16 wysokoscObrazka = wysokosc / 2;
    Uint8 tryb = 1;
    Uint8 dithering = 0;

    cout << "4:2:0 YCbCr" << endl;
    std::ofstream wyjscie = zapiszDoPliku(szerokoscObrazka, wysokoscObrazka, tryb, dithering,0);

    int xx=0,yy=0;
    Uint8 y_tab[szerokosc/2][wysokosc/2];
    Uint8 cb_tab[szerokosc/4][wysokosc/4];
    Uint8 cr_tab[szerokosc/4][wysokosc/4];

    for (int y = 0; y < wysokosc/2; y += 2) {
	    xx=0;
        for (int x = 0; x < szerokosc/2; x += 2) {
            SDL_Color pixel1x = getPixel(x, y);
            SDL_Color pixel2x = getPixel(x + 1, y);
            SDL_Color pixel3x = getPixel(x, y + 1);
            SDL_Color pixel4x = getPixel(x + 1, y + 1);

            YCbCr pixel1 =RGBToYCbCr(pixel1x);
            YCbCr pixel2 =RGBToYCbCr(pixel2x);
            YCbCr pixel3 =RGBToYCbCr(pixel3x);
            YCbCr pixel4 =RGBToYCbCr(pixel4x);

            y_tab[x][y] = pixel1.y;
            y_tab[x+1][y] = pixel2.y;
            y_tab[x][y+1] = pixel3.y;
            y_tab[x+1][y+1] = pixel4.y;

            Uint8 Cb_avg = (pixel1.cb + pixel3.cb+pixel2.cb + pixel4.cb) / 4;
            cb_tab[x][y] = Cb_avg;

            Uint8 Cr_avg = (pixel1.cr + pixel3.cr+pixel2.cr + pixel4.cr) / 4;
            cr_tab[x][y] = Cr_avg;
	    xx++;
        }
	yy++;
    }
    for (int y = 0; y < wysokoscObrazka; y++) {
        for (int x = 0; x < szerokoscObrazka; x++) {
            wyjscie.write((char*)&y_tab[x][y], sizeof(Uint8));
            //cout<<"y_tab["<<x<<"]["<<y<<"]:"<<(int)y_tab[x][y]<<" ";
        }
        //cout<<endl;
    }
    for (int y = 0; y < wysokoscObrazka/2; y++) {
        for (int x = 0; x < szerokoscObrazka/2; x++) {
            wyjscie.write((char*)&cb_tab[x][y], sizeof(Uint8));
        }
    }
    for (int y = 0; y < wysokoscObrazka/2; y++) {
        for (int x = 0; x < szerokoscObrazka/2; x++) {
            wyjscie.write((char*)&cr_tab[x][y], sizeof(Uint8));
        }
    }

    wyjscie.close();

}

Uint8 konwertujNaSzary(SDL_Color color){
    return 0.299*color.r+0.587*color.g+0.114*color.b;
}
