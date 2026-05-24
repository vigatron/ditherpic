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

#include "sysfuncs.hpp"

const char * QStrToStr(QString str) {
    QByteArray byteArray = str.toUtf8();
    const char * c_str = byteArray.constData();
    return c_str;
}

int round4(int v) {
    if(v % 4) { v &= ~3; v += 4; }
    return v;
}

bool SaveMemoryToFile(uint8_t *psrc, const char *fname, int memsz) {
    FILE *wf = fopen(fname, "wb");
    fwrite(psrc, memsz, 1, wf);
    fclose(wf);
    return true;
}

uint32_t COLORBGR(int16_t b, int16_t g, int16_t r) {
    uint32_t bgr = 0;
    bgr |= b; bgr <<= 8;
    bgr |= g; bgr <<= 8;
    bgr |= r; bgr <<= 0;
    return bgr;
}