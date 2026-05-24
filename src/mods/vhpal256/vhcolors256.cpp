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

#include "vhcolors256.hpp"

#include <array>
#include <vector>

namespace VHCLR256 {

static constexpr std::array<uint32_t, 256> arr_bgr24 = [] {
std::array<uint32_t,256> v;
for(int i=0;i<256;i++) v[i] = clr32(i);
return v; }();

uint32_t GetBGR(uint8_t i) {
    return VHCLR256::arr_bgr24[i];
}

void GetErr(uint32_t bgr, uint8_t idx, int16_t *dr, int16_t *dg, int16_t *db) {

    uint8_t r = ( bgr >> 0 ) & 0xFF;
    uint8_t g = ( bgr >> 8 ) & 0xFF;
    uint8_t b = ( bgr >> 16) & 0xFF;

    uint8_t clrr = ( arr_bgr24[idx] >> 0  ) & 0xFF;
    uint8_t clrg = ( arr_bgr24[idx] >> 8  ) & 0xFF;
    uint8_t clrb = ( arr_bgr24[idx] >> 16 ) & 0xFF;

    *dr = r - clrr;
    *dg = g - clrg;
    *db = b - clrb;
}

};
