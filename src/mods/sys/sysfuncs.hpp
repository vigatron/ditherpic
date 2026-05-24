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

#pragma once

#include <QString>

const char *    QStrToStr(QString str);
int             round4(int v);
bool            SaveMemoryToFile(uint8_t *psrc, const char *fname, int memsz);
uint32_t        COLORBGR(int16_t b, int16_t g, int16_t r);
