/*
 * Application : Retro Dithering Utility
 * Author      : Viktor Glebov / V01G04A81
 * Revision    : 0.1.1
 * Source      : https://github.com/vigatron/ditherpic
 * License     : MIT
 * Disclaimer  : Provided "AS IS", without warranty.
 * Encoding    : UTF-8
 * Copyright   : © 2025–2026 Viktor Glebov
 */

#pragma once

#include "vhqt.hpp"

#include "vhpic256/vhpic256.hpp"
#include "wdgimagepanel/wdgimagepanel.hpp"
#include "wdgsettings/wdgsettings.hpp"

#include "dithering.hpp"

#include "vhwdgcam.hpp"

class WidgetDitheringWnd : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDitheringWnd(QWidget *parent = nullptr);

private slots:

    //
    void OnLoadSource();

    //
    void OnConvert();

    //
    void OnSaveResult();

    //
    void OnCameraClick();

private slots:

    // Event from button
    // void OnClickCameraStart();

    // Event from button
    // void OnClickCameraStop();

    // Event from VHWidgetCamera
    void onFrameReady(const QImage &img);

private:

    // Widgets List
    WidgetImagePanel *wdgSrcPanel;
    WidgetSettings *wdgSettings;
    WidgetImagePanel *wdgDstPanel;

    VHWidgetCamera *wdgCamera;
    QPushButton *wdgPushButtonCamera;

    VHDithering objDither;

    void Init();
    void SetupLayout();

    bool LoadSourceImage(const QString &fname);
    bool SaveResults(const QString &fname);

    QString fsrcname;
    VHPIC256 vhpic;
    QImage imgresult;
};
