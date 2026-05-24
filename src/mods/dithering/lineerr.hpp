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

class VHLineErr
{
    public:

        VHLineErr() {
            Clear();
        }

        void Init(int width) {
            arr_r.assign(width, 0);
            arr_g.assign(width, 0);
            arr_b.assign(width, 0);
        }

        const std::size_t width() const { return arr_r.size(); }

        void Clear() {
            for(int i=0;i<arr_r.size();i++) {
                arr_r[i] = 0; arr_g[i] = 0; arr_b[i] = 0;
            }
        }

        void inject(int x, int16_t dr, int16_t dg, int16_t db, int m ) {
            arr_r[x] += m * dr;
            arr_g[x] += m * dg;
            arr_b[x] += m * db;
        }

        uint8_t clamp0to255(int16_t val) {
            uint8_t r;
            if(val < 0) r = 0;                  // Clamp negative
            else if(val > 255) { r = 255; }     // Clamp positive
            else r = val;                       // Matched range
            return r;
        }

        void corr(int BGR, int x, int divisor, uint8_t * pr, uint8_t * pg, uint8_t * pb) {
            * pr = clamp0to255( (( BGR >>  0 ) & 0xFF) + (arr_r[x] / divisor) );
            * pg = clamp0to255( (( BGR >>  8 ) & 0xFF) + (arr_g[x] / divisor) );
            * pb = clamp0to255( (( BGR >> 16 ) & 0xFF) + (arr_b[x] / divisor) );
        }

        const int32_t r(int x) const { return arr_r[x]; }
        const int32_t g(int x) const { return arr_g[x]; }
        const int32_t b(int x) const { return arr_b[x]; }

        void set(int x, int32_t r, int32_t g, int32_t b) {
            arr_r[x] = r; arr_g[x] = g; arr_b[x] = b;
        }

    private:
        std::vector<int32_t> arr_r;
        std::vector<int32_t> arr_g;
        std::vector<int32_t> arr_b;
};
