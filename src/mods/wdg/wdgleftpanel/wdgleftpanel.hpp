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

#include "vhwdgcamimg.hpp"

#include "vhwdgcam.hpp"
#include <qimage.h>

class VHWidgetLeftPanel : public QWidget
{

    Q_OBJECT

public:
    /**
     * @brief Конструктор панели исходного изображения
     * @param txtLabel Текст подписи над изображением
     * @param txtButton Текст кнопки загрузки изображения
     * @param parent Родительский виджет
     */
    explicit VHWidgetLeftPanel(
        const QString &txtLabel,
        const QString &txtButton,
        QWidget *parent = nullptr);

    /**
     * @brief Установить подпись и текст кнопки загрузки
     * Пустые строки игнорируются: соответствующий текст остаётся прежним
     * @param txtLabel Новый текст подписи
     * @param txtButton Новый текст кнопки загрузки
     */
    void setProps(const QString &txtLabel, const QString &txtButton);

    /**
     * @brief Загрузить изображение из файла
     * @param fname Путь к файлу изображения
     * @return true при успешной загрузке, иначе false
     */
    bool setImage(const QString &fname);

    /**
     * @brief Установить готовое изображение
     * Точка входа для кадра камеры: передаёт его в VHWidgetCamImage::SetImage()
     * @param qimage Изображение (например, кадр от VHWidgetCamera)
     * @return true если изображение принято, false для пустого изображения
     */
    bool setImage(const QImage &qimage);

    /**
     * @brief Получить текущее изображение панели
     * @return Ссылка на изображение, хранимое виджетом VHWidgetCamImage
     */
    const QImage &getImage() const { return wdgimage->GetQImage(); }

private:
    /**
     * @brief Создать дочерние виджеты панели
     * @param txtLabel Текст подписи над изображением
     * @param txtButton Текст кнопки загрузки изображения
     */
    void Init(const QString &txtLabel, const QString &txtButton);

    /**
     * @brief Собрать layout панели
     */
    void SetupLayout();

signals:

    void buttonClicked();
    void cameraClicked();
    void captureClicked();

private slots:

    void onclick() { emit buttonClicked(); }
    void onclickCamera() { emit cameraClicked(); }
    void onclickCapture() { emit captureClicked(); }

private:
    QLabel *lbl;
    VHWidgetCamImage *wdgimage;
    QPushButton *btnLoad;
    QPushButton *btnCamera;
    QPushButton *btnCapture;
};
