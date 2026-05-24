/*
 * Application : Retro Dithering Utility
 * Author      : Viktor Glebov / V01G04A81
 * Revision    : 1.0-test
 * Source      : https://github.com/vigatron/ditherpic
 * License     : MIT
 * Disclaimer  : Provided "AS IS", without warranty.
 * Encoding    : UTF-8
 * Copyright   : © 2025–2026 Viktor Glebov
 */

#include "dithering.hpp"

#include "sysfuncs.hpp"

#include "tblfilters.hpp"
#include "tblcolors.hpp"
#include "tblreso.hpp"

// Shift dlines
void VHDithering::ShiftDerrLinesUp() {

    int wdt = lineerr[0].width();

    // Move lines up
    for(int j = 0; j < (4-1); j++) {
        int yf = j+1;
        int yt = j;
        for(int i = 0; i < wdt; i++) {
            lineerr[yt].set(i, lineerr[yf].r(i), lineerr[yf].g(i), lineerr[yf].b(i) );
        }
    }

    // Fill last line with Zeros
    lineerr[3].Clear();
}



uint8_t GetClosest(uint32_t bgr, const std::vector<uint8_t> & palette) {

    uint8_t r = ( bgr >> 0 ) & 0xFF;
    uint8_t g = ( bgr >> 8 ) & 0xFF;
    uint8_t b = ( bgr >> 16) & 0xFF;

    uint32_t bestdelta = 0xFFFFFFFF;
    uint8_t  bestidx = 0;

    for(int i=0; i<palette.size(); i++) {

        uint8_t     colpal = palette[i];
        uint32_t    bgr24  = VHCLR256::GetBGR(colpal);

        uint8_t clrr = ( bgr24 >> 0  ) & 0xFF;
        uint8_t clrg = ( bgr24 >> 8  ) & 0xFF;
        uint8_t clrb = ( bgr24 >> 16 ) & 0xFF;

        int dr = r - clrr;
        int dg = g - clrg;
        int db = b - clrb;

        uint32_t delta = ( dr * dr ) + ( dg * dg ) + ( db * db );
        if(delta < bestdelta) {
            bestdelta = delta;
            bestidx = i;
        }
    }

    return palette[bestidx];
}


void VHDithering::Dithering(const QImage & srcimg, VHPIC256 & vhpic, int idxflt, int idxpal) {

    if(idxflt >= VHDither::count) {
        printf("Invalid filter idx %d \n", idxflt);
        return; }

    if(idxpal >= VHColors::count) {
        printf("Invalid palette idx %d \n", idxpal);
        return; }

    const std::vector<uint8_t> & palette = VHColors::table[idxpal];
    

    const int pxoffs = 2;
    const int widthf    = srcimg.width()    + 2 * pxoffs;

    // Filter settings
    const std::array<uint8_t, 12> fltarr = VHDither::tblFilters[idxflt].weights;
    int divm = VHDither::tblFilters[idxflt].divisor;

    vhpic.Init( srcimg.width(), srcimg.height());

    // x4 Errors Buffers Lines / RGB Colors delta
    for(int i=0; i<4; i++) { lineerr[i].Init(widthf+4); }

    for( int y=0; y < srcimg.height(); y++) {

        ShiftDerrLinesUp();

        for( int x=0; x < srcimg.width(); x++ ) {

            // src pixel
            QRgb bgro = srcimg.pixel(x, y);

            // Get clamped r, g, b
            uint8_t r,g,b;
            lineerr[0].corr( bgro, x+2, divm, &r, &g, &b);

            uint32_t    bgr = COLORBGR(b, g, r);
            uint8_t     nearest = GetClosest(bgr, palette);

            // Put result
            vhpic.Set(x, y, nearest);

            // Inject errors dithering delta
            int16_t dr, dg, db;
            VHCLR256::GetErr(bgr, nearest, &dr, &dg, &db);

            for(int i=0;i<12;i++) {
                int m  = fltarr[i];
                if(m) {
                    int nx = x + 2 + VHDither::dx[i];
                    int ny = VHDither::dy[i];
                    lineerr[ny].inject(nx, dr, dg, db, m);
                }
            }

        }
    }
}
