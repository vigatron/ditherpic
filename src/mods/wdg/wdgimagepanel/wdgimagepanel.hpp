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

#include "wdgimage/wdgimage.hpp"
#include <qimage.h>

class WidgetImagePanel : public QWidget {

    Q_OBJECT

    public:

        explicit        WidgetImagePanel(
            const QString & txtLabel,
            const QString & txtButton,
            QWidget * parent=nullptr);

        void            setProps(const QString & txtLabel, const QString & txtButton);

        bool            setImage(const QString & fname);
        bool            setImage(const QImage  & qimage);

        const QImage &  getImage() const { return wdgimage->GetQImage(); }

    private:

        void            Init(const QString & txtLabel, const QString & txtButton);
        void            SetupLayout();

    signals:

        void            buttonClicked();

    private slots:

        void            onclick() { emit buttonClicked(); }

    private:

        QLabel      *   lbl;
        WidgetImage *   wdgimage;
        QPushButton *   btn;

};
