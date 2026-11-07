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
#include "wdgleftpanel/wdgleftpanel.hpp"
#include "wdgrightpanel/wdgrightpanel.hpp"
#include "wdgsettings/wdgsettings.hpp"

#include "dithering.hpp"

#include "vhwdgcam.hpp"

class QCloseEvent;

class WidgetDitheringWnd : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор главного окна
     * @param parent Родительский виджет
     */
    explicit WidgetDitheringWnd(QWidget *parent = nullptr);

    /**
     * @brief Деструктор
     * Закрывает окно камеры и освобождает её ресурсы, чтобы устройство
     * не осталось занятым после завершения приложения
     */
    ~WidgetDitheringWnd();

protected:

    /**
     * @brief Обработчик закрытия главного окна
     * Закрывает окно камеры: камера деинициализируется, даже если
     * пользователь не нажимал Camera Stop
     * @param event Событие закрытия
     */
    void closeEvent(QCloseEvent *event) override;

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
    VHWidgetLeftPanel *wdgSrcPanel;
    WidgetSettings *wdgSettings;
    VHWidgetRightPanel *wdgDstPanel;

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
