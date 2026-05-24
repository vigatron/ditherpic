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

#include <array>
#include <string_view>

namespace VHReso {

struct stReso {
    std::string_view name;
    std::string_view aspc;
    int w;
    int h;
};

inline constexpr int count = 13;

inline constexpr std::array<stReso,count> tblResolutions {{
    { "Original",   "",        0,    0 },
    { "WUXGA",  "16:10",    1920, 1200 },
    { "UXGA",     "4:3",    1600, 1200 },
    { "FHD",     "16:9",    1920, 1080 },
    { "SXGA",     "5:4",    1280, 1024 },
    { "XGA",      "4:3",    1024,  768 },
    { "SVGA",     "4:3",     800,  600 },
    { "WVGA",     "5:3",     800,  480 },
    { "VGA",      "4:3",     640,  480 },
    { "WQVGA",    "5:3",     400,  240 },
    { "QVGA",     "4:3",     320,  240 },
    { "DOS",      "8:5",     320,  200 },
    { "ZX",       "4:3",     256,  192 }
}};

};
