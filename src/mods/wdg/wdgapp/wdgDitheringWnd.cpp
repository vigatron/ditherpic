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

#include "wdgDitheringWnd.hpp"
#include "wdg/separators.hpp"

#include "vhwdgcam.hpp"

#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qpushbutton.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDateTime>
#include <QDir>

#include <QGraphicsDropShadowEffect>

#include "sys/sysfuncs.hpp"

#include "tblfilters.hpp"
#include "tblreso.hpp"
#include "tblcolors.hpp"

#include <QImageWriter>

/**
 *
 */
WidgetDitheringWnd::WidgetDitheringWnd(QWidget *parent)
    : QWidget(parent)
{

    Init();
}

/**
 * @brief Деструктор
 * Закрывает окно камеры и освобождает её ресурсы
 */
WidgetDitheringWnd::~WidgetDitheringWnd()
{
    if (wdgCamera)
    {
        // Останавливает поток кадров и освобождает пайплайн захвата
        wdgCamera->close();

        delete wdgCamera;
        wdgCamera = nullptr;
    }
}

/**
 * @brief Обработчик закрытия главного окна
 * Закрывает окно камеры, чтобы камера была деинициализирована,
 * даже если пользователь не нажимал Camera Stop
 * @param event Событие закрытия
 */
void WidgetDitheringWnd::closeEvent(QCloseEvent *event)
{
    if (wdgCamera)
    {
        wdgCamera->close();
    }

    QWidget::closeEvent(event);
}

/**
 *
 */
void WidgetDitheringWnd::Init()
{

    wdgSrcPanel = new VHWidgetLeftPanel("Source File Name", "Load Picture ...", this);
    connect(wdgSrcPanel, &VHWidgetLeftPanel::buttonClicked, this, &WidgetDitheringWnd::OnLoadSource);

    wdgSettings = new WidgetSettings(this);
    wdgSettings->setEnabled(false);
    connect(wdgSettings, &WidgetSettings::StartConversion, this, &WidgetDitheringWnd::OnConvert);

    wdgDstPanel = new VHWidgetRightPanel("Result File Name", "Save Results", this);
    wdgDstPanel->setEnabled(false);
    connect(wdgDstPanel, &VHWidgetRightPanel::buttonClicked, this, &WidgetDitheringWnd::OnSaveResult);

    // wdgCamera
    wdgCamera = new VHWidgetCamera();
    wdgCamera->setWindowTitle("Camera Preview");

    // Camera errors are reported to the user. Frame events are not connected:
    // the camera keeps the last frame itself (VHWidgetCamera::currentImage()),
    // so only CaptureImage() puts a frame into the source panel
    connect(
        wdgCamera, &VHWidgetCamera::errorOccurred,
        this, &WidgetDitheringWnd::OnCameraError );

    // Camera button events are emitted via VHWidgetLeftPanel:
    // btnCamera -> cameraClicked -> OnCameraClick, btnCapture -> captureClicked -> OnCaptureFrame
    connect(
        wdgSrcPanel, &VHWidgetLeftPanel::cameraClicked,
        this, &WidgetDitheringWnd::OnCameraClick );

    connect(
        wdgSrcPanel, &VHWidgetLeftPanel::captureClicked,
        this, &WidgetDitheringWnd::OnCaptureFrame );

    SetupLayout();
}

/**
 *
 */
void WidgetDitheringWnd::SetupLayout()
{

    // Left Side ( Image View + Camera settings )
    auto vlayleft = new QVBoxLayout();
    vlayleft->addWidget(wdgSrcPanel);

    // Main Layout
    auto hlay = new QHBoxLayout(this);

    hlay->addLayout(vlayleft);
    // hlay->addWidget(wdgSrcPanel);

    hlay->addWidget(new VLineSep());
    hlay->addWidget(wdgSettings);
    hlay->addWidget(new VLineSep());
    hlay->addWidget(wdgDstPanel);
}

/**
 *
 */
bool convertVHPICtoQImage(const VHPIC256 &vhpic, QImage &dstimg)
{

    // Prepare memory buffer
    const int worig = vhpic.width();
    const int horig = vhpic.height();
    const int ALIGNED_W = (worig % 4) ? ((worig & ~3) + 4) : worig;
    const int BUFFER_SZ = ALIGNED_W * horig;

    const std::vector<uint8_t> srcarr = vhpic.data();
    std::vector<uint8_t> dstarr(BUFFER_SZ, 0);

    // Copy line to aligned line (mod 4)
    for (int line = 0; line < horig; line++)
    {
        int srcoffs = line * worig;
        int dstoffs = line * ALIGNED_W;
        std::copy(
            srcarr.begin() + srcoffs,
            srcarr.begin() + srcoffs + worig,
            dstarr.begin() + dstoffs);
    }

    // Create result
    QImage image(dstarr.data(), worig, horig, QImage::Format_Indexed8);

    // Set 8-bit palette
    QVector<QRgb> colors;
    for (int i = 0; i < 256; i++)
    {
        colors.push_back(VHCLR256::GetBGR(i));
    }
    image.setColorCount(256);
    image.setColorTable(colors);

    dstimg = image.copy();

    return true;
}

/**
 *
 */
bool WidgetDitheringWnd::LoadSourceImage(const QString &fname)
{

    bool r = wdgSrcPanel->setImage(fname);
    if (!r)
        return r;

    const QImage &img = wdgSrcPanel->getImage();
    QString reso = QString::number(img.width()) + "x" + QString::number(img.height());
    QFileInfo info(fname);
    QString props = reso + " : " + info.fileName();

    wdgSrcPanel->setProps(props, "");

    return r;
}

/**
 *
 */
void WidgetDitheringWnd::OnLoadSource()
{

    fsrcname = QFileDialog::getOpenFileName(
        this,
        tr("Выберите изображение"),
        QString(), // начальная папка
        tr("Изображения (*.png *.jpg *.JPG *.jpeg *.bmp)"));

    // No file selected ?
    if (fsrcname.isEmpty())
        return;

    bool r = LoadSourceImage(fsrcname);
    wdgSettings->setEnabled(r);

    wdgDstPanel->setEnabled(false);
}

/**
 *
 */
void WidgetDitheringWnd::OnConvert()
{

    int idxflt = wdgSettings->idxflt();
    if (idxflt < 0)
        return;

    int idxpal = wdgSettings->idxpal();
    if (idxpal < 0 || idxpal >= VHColors::count)
        return;

    if (!wdgSrcPanel->getImage().width())
        return;

    printf("Conversion started\n");

    // Check resolution / rescale enabled
    int idxreso = wdgSettings->idxreso();
    int resox = VHReso::tblResolutions[idxreso].w;
    int resoy = VHReso::tblResolutions[idxreso].h;

    bool flag = !resox || !resoy;
    QImage srcimg = flag ? wdgSrcPanel->getImage().copy() : wdgSrcPanel->getImage().scaled(resox, resoy, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    // Dithering Process
    objDither.Dithering(srcimg, vhpic, idxflt, idxpal);

    // VHPIX to QImage (.png)
    convertVHPICtoQImage(vhpic, imgresult);
    printf("Conversion completed\n");

    // Show result
    wdgDstPanel->setImage(imgresult);
    wdgDstPanel->setEnabled(true);
}

/**
 *
 */
void WidgetDitheringWnd::OnSaveResult()
{

    qDebug() << QImageWriter::supportedImageFormats();

    if (!vhpic.width())
        return;

    // Prepare files names
    QFileInfo fi(fsrcname);
    QString base = fi.completeBaseName();
    QString fnamevhpic = fi.path() + "/" + base + "_out.vhpic";
    QString fnamepng = fi.path() + "/" + base + "_out.png";

    // Save VHPix (.vhpic)
    vhpic.Save(fnamevhpic);
    printf("Saved %s \n", QStrToStr(fnamevhpic));

    // Save PNG (.png)
    QImage rgbImg = imgresult.convertToFormat(QImage::Format_RGB32);
    rgbImg.save(fnamepng);
    printf("Saved %s \n", QStrToStr(fnamepng));
}

/**
 *
 */
bool WidgetDitheringWnd::SaveResults(const QString &fname)
{
    return true;
}

/**
 * @brief Захватить последний кадр камеры в панель исходного изображения
 * Последний кадр берётся у окна камеры (VHWidgetCamera::currentImage()) и
 * передаётся в VHWidgetLeftPanel, который вызывает VHWidgetCamImage::SetImage().
 * Имя camera_<timestamp> становится источником для Save Results
 * @return true - кадр передан в панель; false - кадра нет или SetImage() отказал
 */
bool WidgetDitheringWnd::CaptureImage()
{
    if (!wdgCamera)
        return false;

    // Кадр от VHWidgetCamera (submodule vhlibcamera)
    const QImage &img = wdgCamera->currentImage();
    if (img.isNull())
    {
        QMessageBox::information(
            this,
            tr("Camera"),
            tr("Нет кадра: включите камеру (Turn On Camera) и дождитесь превью."));
        return false;
    }

    // Кадр камеры -> VHWidgetLeftPanel -> VHWidgetCamImage::SetImage()
    if (!wdgSrcPanel->setImage(img))
        return false;

    const QImage &src = wdgSrcPanel->getImage();
    wdgSrcPanel->setProps(
        QString::number(src.width()) + "x" + QString::number(src.height()) + " : Camera", "");

    // Источник для Save Results: camera_<timestamp>
    fsrcname = QDir::home().filePath(
        "camera_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss"));

    wdgSettings->setEnabled(true);
    wdgDstPanel->setEnabled(false);

    return true;
}

/**
 * @brief Обработчик нажатия кнопки Capture Frame
 * Захватывает последний кадр камеры в исходную панель
 */
void WidgetDitheringWnd::OnCaptureFrame()
{
    CaptureImage();
}

/**
 * @brief Обработчик нажатия кнопки Turn On Camera
 * Показывает окно камеры
 */
void WidgetDitheringWnd::OnCameraClick()
{
    wdgCamera->show();
}

/**
 * @brief Обработчик ошибок камеры
 * Показывает пользователю сообщение, полученное от VHWidgetCamera
 * @param msg Текст ошибки
 */
void WidgetDitheringWnd::OnCameraError(const QString &msg)
{
    QMessageBox::warning(this, tr("Camera"), msg);
}
