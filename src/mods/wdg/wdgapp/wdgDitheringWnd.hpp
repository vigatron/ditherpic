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

#include "vhpic256/vhpic256.hpp"
#include "wdgimage/wdgimage.hpp"
#include "wdgimagepanel/wdgimagepanel.hpp"
#include "wdgsettings/wdgsettings.hpp"

#include "dithering.hpp"


class WidgetDitheringWnd : public QWidget
{
    Q_OBJECT

public:

    explicit    WidgetDitheringWnd(QWidget * parent = nullptr);

protected:

private slots:

    void        OnLoadSource();
    void        OnConvert();
    void        OnSaveResult();

private:

    WidgetImagePanel    *   wdgSrcPanel;
    WidgetSettings      *   wdgSettings;
    WidgetImagePanel    *   wdgDstPanel;

    VHDithering             objDither;

    void                    Init();
    void                    SetupLayout();

    bool                    LoadSourceImage (const QString & fname);
    bool                    SaveResults     (const QString & fname);

    QString                 fsrcname;
    VHPIC256                vhpic;
    QImage                  imgresult;

};

