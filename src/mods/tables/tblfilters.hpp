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

namespace VHDither {

inline constexpr int count = 8;
inline constexpr int depth = 12;

struct Filter {
    std::string_view name;                 // Опционально: имя фильтра для удобства
    std::array<uint8_t, depth> weights;    // Матрица распределения ошибки
    int divisor;                           // Делитель для этого фильтра
};

// Константы времени компиляции (constexpr)
inline constexpr std::array<Filter, count> tblFilters{{
    { "1. Floyd-Steinberg",      { 7,0,  0,3,5,1,0,  0,0,0,0,0 }, 16 },
    { "2. Jarvis-Judice-Ninke",  { 7,5,  3,5,7,5,3,  1,3,5,3,1 }, 48 },
    { "3. Stucki",               { 8,4,  2,4,8,4,2,  1,2,4,2,1 }, 42 },
    { "4. Burkes",               { 8,4,  2,4,8,4,2,  0,0,0,0,0 }, 32 },
    { "5. Atkinson",             { 1,1,  0,1,1,1,0,  0,0,1,0,0 }, 8  },
    { "6. Sierra 3-row",         { 5,3,  2,4,5,4,2,  0,2,3,2,0 }, 32 },
    { "7. Sierra 2-row",         { 4,3,  1,2,3,2,1,  0,0,0,0,0 }, 16 },
    { "8. Sierra Lite",          { 2,0,  0,1,1,0,0,  0,0,0,0,0 }, 4  }
}};

// Matrix X-Y offset array
inline constexpr std::array<int8_t,depth> dx {  1,  2, -2, -1,  0,  1,  2, -2, -1,  0,  1,  2 };
inline constexpr std::array<int8_t,depth> dy {  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2 };

} // namespace Dithering
