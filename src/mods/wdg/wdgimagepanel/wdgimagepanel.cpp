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

#include "wdgimagepanel.hpp"

WidgetImagePanel::WidgetImagePanel(
    const QString & txtLabel,
    const QString & txtButton,
    QWidget * parent)
    : QWidget(parent) {

        Init(txtLabel, txtButton);
        SetupLayout();
}

void WidgetImagePanel::setProps(const QString & txtLabel, const QString & txtButton) {
    if(!txtLabel.isEmpty()) lbl->setText(txtLabel);
    if(!txtButton.isEmpty()) btn->setText(txtButton);
}

bool WidgetImagePanel::setImage(const QString & fname) {
    return wdgimage->loadimage(fname);
}

bool WidgetImagePanel::setImage(const QImage  & qimage) {
    return wdgimage->setimage(qimage);
}

void WidgetImagePanel::Init(const QString & txtLabel, const QString & txtButton) {

    lbl = new QLabel(txtLabel, this);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    wdgimage = new WidgetImage();

    btn = new QPushButton(txtButton, this);
    connect(btn, &QPushButton::clicked, this, &WidgetImagePanel::onclick);

}

void WidgetImagePanel::SetupLayout() {

    auto play = new QVBoxLayout();
    play->setContentsMargins(16, 16, 16, 16);
    play->addWidget(wdgimage);

    auto vlay = new QVBoxLayout(this);
    vlay->addWidget(lbl, Qt::AlignTop);
    vlay->addLayout(play);
    vlay->addWidget(btn);

}
