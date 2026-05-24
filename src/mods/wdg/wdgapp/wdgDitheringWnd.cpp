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

#include "wdgDitheringWnd.hpp"
#include "wdg/separators.hpp"
#include "wdg/wdgimage/wdgimage.hpp"

#include <QVBoxLayout>
#include <qboxlayout.h>
#include <qgroupbox.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qnamespace.h>
#include <qpushbutton.h>

#include <QFileDialog>
#include <QMessageBox>

#include <QGraphicsDropShadowEffect>

#include "sys/sysfuncs.hpp"

#include "tblfilters.hpp"
#include "tblreso.hpp"
#include "tblcolors.hpp"

#include <QImageWriter>

WidgetDitheringWnd::WidgetDitheringWnd(QWidget * parent)
    : QWidget(parent) {

    Init();

}

void WidgetDitheringWnd::Init() {

    wdgSrcPanel     = new WidgetImagePanel("Source File Name", "Load Picture ...", this );
    connect(wdgSrcPanel, &WidgetImagePanel::buttonClicked, this, &WidgetDitheringWnd::OnLoadSource);

    wdgSettings     = new WidgetSettings(this);
    wdgSettings->setEnabled(false);
    connect(wdgSettings, &WidgetSettings::StartConversion, this, &WidgetDitheringWnd::OnConvert);

    wdgDstPanel     = new WidgetImagePanel("Result File Name", "Save Results", this);
    wdgDstPanel->setEnabled(false);
    connect(wdgDstPanel, &WidgetImagePanel::buttonClicked, this, &WidgetDitheringWnd::OnSaveResult);

    SetupLayout();
}


void WidgetDitheringWnd::SetupLayout() {

    auto hlay = new QHBoxLayout(this);
    hlay->addWidget(wdgSrcPanel);
    hlay->addWidget(new VLineSep());
    hlay->addWidget(wdgSettings);
    hlay->addWidget(new VLineSep());
    hlay->addWidget(wdgDstPanel);

}


bool convertVHPICtoQImage(const VHPIC256 & vhpic, QImage & dstimg) {

    // Prepare memory buffer
    const int worig = vhpic.width();
    const int horig = vhpic.height();
    const int ALIGNED_W =  (worig % 4) ? ((worig & ~3) + 4) : worig;
    const int BUFFER_SZ = ALIGNED_W * horig;

    const std::vector<uint8_t>  srcarr = vhpic.data();
    std::vector<uint8_t>        dstarr(BUFFER_SZ, 0);

    // Copy line to aligned line (mod 4)
    for( int line = 0; line < horig; line++) {
        int srcoffs = line * worig;
        int dstoffs = line * ALIGNED_W;
        std::copy(
            srcarr.begin() + srcoffs,
            srcarr.begin() + srcoffs + worig,
            dstarr.begin() + dstoffs);
    }

    // Create result
    QImage image(dstarr.data(), worig, horig, QImage::Format_Indexed8 );

    // Set 8-bit palette
    QVector<QRgb> colors;
    for(int i=0;i<256;i++) { colors.push_back( VHCLR256::GetBGR(i)); }
    image.setColorCount(256);
    image.setColorTable(colors);

    dstimg = image.copy();

    return true;
}


void WidgetDitheringWnd::OnLoadSource() {

    fsrcname = QFileDialog::getOpenFileName(
        this,
        tr("Выберите изображение"),
        QString(), // начальная папка
        tr("Изображения (*.png *.jpg *.JPG *.jpeg *.bmp)")
    );

    // No file selected ?
    if (fsrcname.isEmpty())
        return;

    bool r = LoadSourceImage(fsrcname);
    wdgSettings->setEnabled(r);

    wdgDstPanel->setEnabled(false);
}

void WidgetDitheringWnd::OnConvert() {

    int idxflt = wdgSettings->idxflt();
    if(idxflt<0)
        return;

    int idxpal = wdgSettings->idxpal();
    if(idxpal<0 || idxpal >= VHColors::count )
        return;

    if(!wdgSrcPanel->getImage().width())
        return;

    printf("Conversion started\n");

    // Check resolution / rescale enabled
    int idxreso = wdgSettings->idxreso();
    int resox = VHReso::tblResolutions[idxreso].w;
    int resoy = VHReso::tblResolutions[idxreso].h;

    QImage srcimg =  (!resox || !resoy) ?
        wdgSrcPanel->getImage().copy() : 
        wdgSrcPanel->getImage().scaled( resox, resoy, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

    // Dithering Process
    objDither.Dithering(srcimg, vhpic, idxflt, idxpal );

    // VHPIX to QImage (.png)
    convertVHPICtoQImage(vhpic, imgresult);
    printf("Conversion completed\n");

    // Show result
    wdgDstPanel->setImage(imgresult);
    wdgDstPanel->setEnabled(true);

}

void WidgetDitheringWnd::OnSaveResult() {

    qDebug() << QImageWriter::supportedImageFormats();


    if(!vhpic.width())
        return;

    // Prepare files names
    QFileInfo fi(fsrcname);
    QString base = fi.completeBaseName();
    QString fnamevhpic = fi.path() + "/" + base + "_out.vhpic";
    QString fnamepng   = fi.path() + "/" + base + "_out.png";

    // Save VHPix (.vhpic)
    vhpic.Save(fnamevhpic);
    printf("Saved %s \n", QStrToStr(fnamevhpic));

    // Save PNG (.png)
    QImage rgbImg = imgresult.convertToFormat(QImage::Format_RGB32);
    rgbImg.save(fnamepng);
    printf("Saved %s \n", QStrToStr(fnamepng));

}


bool WidgetDitheringWnd::LoadSourceImage(const QString & fname) {

    bool r = wdgSrcPanel->setImage(fname);
    if(!r) return r;

    const QImage &  img  = wdgSrcPanel->getImage();
    QString         reso = QString::number(img.width()) + "x" + QString::number(img.height());
    QFileInfo       info(fname);
    QString         props = reso + " : " + info.fileName();

    wdgSrcPanel->setProps(props, "");

    return r;
}

bool WidgetDitheringWnd::SaveResults(const QString & fname) {
    return true;
}
