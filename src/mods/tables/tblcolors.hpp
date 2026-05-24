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

#include <stdint.h>
#include <array>
#include <string_view>
#include <span>
#include <numeric>

#include "vhcolors256.hpp"


namespace VHColors {


inline constexpr uint8_t EGA0 = 0x00;
inline constexpr uint8_t EGAL = 0x55;
inline constexpr uint8_t EGAM = 0xAA;
inline constexpr uint8_t EGAH = 0xFF;

inline constexpr uint8_t ZXS0 = 0x00;
inline constexpr uint8_t ZXSM = 0xD7;
inline constexpr uint8_t ZXSH = 0xFF;

inline constexpr int count = 7;

inline constexpr std::array<std::string_view, count> names = { 
    "256",
    "64",
    "EGA",
    "ZX",
    "4b Gray",
    "2b Gray",
    "1b B&W"
};

inline const std::vector<uint8_t> arr_256 = [] {
    std::vector<uint8_t> v(256);
    std::iota(v.begin(), v.end(), 0);
    return v; }();

inline const std::vector<uint8_t> arr_64 = [] {
    std::vector<uint8_t> v(64);
    std::iota(v.begin(), v.end(), 0);
    return v; }();

inline constexpr uint8_t cround(uint8_t r, uint8_t g, uint8_t b) {
    uint32_t bestdelta = 0xFFFFFFFF;
    uint8_t  bestidx = 0;
    
    for(int i=0; i<256; i++) {

        uint32_t    bgr24  = VHCLR256::GetBGR(i);
        uint8_t     clrr = ( bgr24 >> 0  ) & 0xFF;
        uint8_t     clrg = ( bgr24 >> 8  ) & 0xFF;
        uint8_t     clrb = ( bgr24 >> 16 ) & 0xFF;
        int         dr = r - clrr;
        int         dg = g - clrg;
        int         db = b - clrb;

        uint32_t delta = ( dr * dr ) + ( dg * dg ) + ( db * db );
        if(delta < bestdelta) {
            bestdelta = delta;
            bestidx = i;
        }
    }

    return bestidx;
}

inline const std::vector<uint8_t> arr_ega = {

    cround(EGA0, EGA0, EGA0),   //  0.  Black
    cround(EGA0, EGA0, EGAM),   //  1.  Blue
    cround(EGA0, EGAM, EGA0),   //  2.  Green
    cround(EGA0, EGAM, EGAM),   //  3.  Cyan

    cround(EGAM, EGA0, EGA0),   //  4.  Red
    cround(EGAM, EGA0, EGAM),   //  5.  Magenta
    cround(EGAM, EGAL, EGA0),   //  6.  Brown
    cround(EGAM, EGAM, EGAM),   //  7.  Light Grey

    cround(EGAL, EGAL, EGAL),   //  8.  Dark Grey
    cround(EGAL, EGAL, EGAH),   //  9.  Light Blue
    cround(EGAL, EGAH, EGAL),   // 10.  Light Green
    cround(EGAL, EGAH, EGAH),   // 11.  Light Cyan

    cround(EGAH, EGAL, EGAL),   // 12.  Light Red
    cround(EGAH, EGAL, EGAH),   // 13.  Light Magenta
    cround(EGAH, EGAH, EGAM),   // 14.  Yellow
    cround(EGAH, EGAH, EGAH),   // 15.  White

};

inline const std::vector<uint8_t> arr_zx  = {

    cround(ZXS0, ZXS0, ZXS0),   //  0.  L Black
    cround(ZXS0, ZXS0, ZXSM),   //  1.  L Blue
    cround(ZXSM, ZXS0, ZXS0),   //  2.  L Red
    cround(ZXSM, ZXS0, ZXSM),   //  3.  L Magenta

    cround(ZXS0, ZXSM, ZXS0),   //  4.  L Green
    cround(ZXS0, ZXSM, ZXSM),   //  5.  L Cyan
    cround(ZXSM, ZXSM, ZXS0),   //  6.  L Yellow
    cround(ZXSM, ZXSM, ZXSM),   //  7.  L White

    cround(ZXS0, ZXS0, ZXS0),   //  8.  H Black
    cround(ZXS0, ZXS0, ZXSH),   //  9.  H Blue
    cround(ZXSH, ZXS0, ZXS0),   // 10.  H Red
    cround(ZXSH, ZXS0, ZXSH),   // 11.  H Magenta

    cround(ZXS0, ZXSH, ZXS0),   // 12.  H Green
    cround(ZXS0, ZXSH, ZXSH),   // 13.  H Cyan
    cround(ZXSH, ZXSH, ZXS0),   // 14.  H Yellow
    cround(ZXSH, ZXSH, ZXSH),   // 15.  H White

};

inline const std::vector<uint8_t> arr_bw16 = {
    0,      // 00
    149,    // 11
    64,     // 22
    213,    // 33
    128,    // 44
    21,     // 55
    192,    // 66
    85,     // 77
    106,    // 88
    255,    // 99
    42,     // aa
    191,    // bb
    234,    // cc
    127,    // dd
    170,    // ee
    63      // ff
};

inline const std::vector<uint8_t> arr_bw4 = { 0, 192, 191, 63 };

inline const std::vector<uint8_t> arr_bw = { 0, 63 };

inline const std::vector<std::vector<uint8_t>> table = {
    arr_256,
    arr_64,
    arr_ega,
    arr_zx,
    arr_bw16,
    arr_bw4,
    arr_bw
};

};
