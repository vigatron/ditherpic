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

#include "wdgimage/wdgimage.hpp"

WidgetImage::WidgetImage(QWidget * parent)
    : QWidget(parent), image(nullptr) {

    setMinimumSize(640, 480);

    // DropShadowEffect для мягкой тени
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect;
    shadow->setBlurRadius(32);
    shadow->setOffset(4, 4);
    shadow->setColor(Qt::darkGray);
    setGraphicsEffect(shadow);

}

bool WidgetImage::loadimage(const QString & fname) {
   
    bool r = image.load(fname);
    update();
    return r;
}

void WidgetImage::paintEvent(QPaintEvent *e) {

    QPainter p(this);

    if(!image.width()) {
        // Заливка всего виджета белым цветом
        // QColor(0x50, 0x50, 0x50)
        QColor defaultColor = palette().color(QPalette::Window);
        QColor textColor    = palette().color(QPalette::Text);
        p.fillRect(rect(), defaultColor);

        // Настройка шрифта и цвета текста
        QFont font = p.font();
        font.setPointSize(12);
        p.setFont(font);
        p.setPen(textColor);

        // Отрисовка текста по центру
        p.drawText(rect(), Qt::AlignCenter, "No Image");
        return;
    }

    QRect bufrect = QRect(0, 0, image.width() , image.height());
    QRect scrrect = rect();
    const int flag_resize = 1;

    QImage convImage = image.convertToFormat(QImage::Format_RGB32, Qt::ThresholdDither | Qt::AutoColor);
    p.drawImage(scrrect, convImage, bufrect);

}

void WidgetImage::mousePressEvent(QMouseEvent *e) {
    // int xq = 0;
    // int yq = 0;
    // int sQ = 64;
    // QPointF pos = e->position();
    // int mousex = qRound(pos.x()), mousey = qRound(pos.y());
    // xq = mousex / sQ;
    // yq = mousey / sQ;
    // printf("%d %d  %d:%d\n", mousex, mousey, xq, yq);
}


bool WidgetImage::setimage(const QImage & newImage) {

    // нельзя установить пустое изображение
    if (newImage.isNull()) {
        return false;
    }

    // создаём копию + проверка на случай нехватки памяти
    image = newImage.copy();

    update();
    return true;
}
