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

#include "vhqt.hpp"


class WidgetImage : public QWidget {

    public:

        explicit WidgetImage(QWidget * parent=nullptr);

        bool                loadimage   (const QString & fname);
        bool                setimage    (const QImage & newImage);
        const   QImage &    GetQImage   () const { return image; }

    protected:

        void paintEvent(QPaintEvent *e) override;
        void mousePressEvent(QMouseEvent *e) override;

    private:
        QImage image;
};
