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
#include <vector>

#include "sysfuncs.hpp"

class VHPIC256
{
    public:

        explicit VHPIC256() : w(0), h(0) { }

        struct sthdr {
            uint8_t     hdr[4];
            uint16_t    wdth;
            uint16_t    hght;
        } __attribute__((packed));

        void Init(int w, int h) {
            this->w = w;
            this->h = h;
            pxls = w * h;
            arrdata.assign(pxls, 0);
        }

        void Set(int x, int y, int v) { 
            int offs = offset(x,y);
            if(offs < pxls)
                arrdata[offs] = v;
        }

        unsigned char Get(int x, int y) const {
            int offs = offset(x,y);
            if(offs < pxls)
                return arrdata[offs];
            // ASSERT
            return 0;
        }

        const uint8_t *             GetPixData()    const   { return arrdata.data(); }
        const std::vector<uint8_t>  data()          const   { return arrdata; }
        uint16_t                    width()         const   { return w; }
        uint16_t                    height()        const   { return h; }
        uint32_t                    pixels()        const   { return w * h; }  

        void            Save(const QString & fname, uint32_t opts=0) const {

            VHPIC256::sthdr hdr;
            hdr.hdr[0] = 'V'; hdr.hdr[1] = 'H'; hdr.hdr[2] = 'P'; hdr.hdr[3] = 'X';
            hdr.wdth = width();
            hdr.hght = height();

            FILE *wf = fopen( QStrToStr(fname), "wb");
            fwrite(&hdr, sizeof(VHPIC256::sthdr), 1, wf);
            fwrite(GetPixData(), pixels(), 1, wf );
            fclose(wf);
        }

    private:

        int             offset(int x, int y) const { return (y * w) + x; }

        std::vector<uint8_t>  arrdata;

        int w;
        int h;
        int pxls;

};
