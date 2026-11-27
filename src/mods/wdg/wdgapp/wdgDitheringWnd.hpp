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

    /**
     * @brief Обработчик нажатия кнопки Load Picture
     * Открывает диалог выбора файла и загружает изображение в исходную панель
     */
    void OnLoadSource();

    /**
     * @brief Обработчик запуска конвертации
     * Применяет выбранный фильтр dithering и палитру к исходному изображению
     */
    void OnConvert();

    /**
     * @brief Обработчик сохранения результата
     * Сохраняет результат в форматах .vhpic и .png
     */
    void OnSaveResult();

    /**
     * @brief Обработчик нажатия кнопки Capture Frame
     * Захватывает кадр камеры в исходную панель через CaptureImage()
     */
    void OnCaptureFrame();

    /**
     * @brief Обработчик нажатия кнопки Turn On Camera
     * Показывает окно камеры VHWidgetCamera
     */
    void OnCameraClick();

    /**
     * @brief Обработчик ошибок камеры
     * Показывает пользователю сообщение, полученное от VHWidgetCamera
     * @param msg Текст ошибки
     */
    void OnCameraError(const QString &msg);

private:

    // Widgets List
    VHWidgetLeftPanel *wdgSrcPanel;
    WidgetSettings *wdgSettings;
    VHWidgetRightPanel *wdgDstPanel;

    VHWidgetCamera *wdgCamera;

    VHDithering objDither;

    void Init();
    void SetupLayout();

    /**
     * @brief Захватить последний кадр камеры в панель исходного изображения
     * Берёт кадр у VHWidgetCamera (currentImage()), передаёт его в
     * VHWidgetLeftPanel, который вызывает VHWidgetCamImage::SetImage().
     * Источником для Save Results становится имя camera_<timestamp>
     * @return true - кадр передан в панель; false - кадра нет или SetImage() отказал
     */
    bool CaptureImage();

    bool LoadSourceImage(const QString &fname);
    bool SaveResults(const QString &fname);

    QString fsrcname;
    VHPIC256 vhpic;
    QImage imgresult;
};
