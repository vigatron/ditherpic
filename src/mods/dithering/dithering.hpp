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
#include <QImage>

#include "lineerr.hpp"
#include "vhcolors256.hpp"
#include "vhpic256.hpp"

class VHDithering {

    public:

        void Dithering(const QImage & srcimg, VHPIC256 & vhpic, int idxflt, int idxpal);

    private:

        VHLineErr lineerr[4];
        void ShiftDerrLinesUp();
};
