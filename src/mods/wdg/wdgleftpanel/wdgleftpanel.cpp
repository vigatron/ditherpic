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

#include "wdgleftpanel.hpp"

VHWidgetLeftPanel::VHWidgetLeftPanel(
    const QString & txtLabel,
    const QString & txtButton,
    QWidget * parent)
    : QWidget(parent) {

        Init(txtLabel, txtButton);
        SetupLayout();
}

void VHWidgetLeftPanel::setProps(const QString & txtLabel, const QString & txtButton) {
    if(!txtLabel.isEmpty()) lbl->setText(txtLabel);
    if(!txtButton.isEmpty()) btn->setText(txtButton);
}

bool VHWidgetLeftPanel::setImage(const QString & fname) {
    return wdgimage->LoadImage(fname);
}

bool VHWidgetLeftPanel::setImage(const QImage  & qimage) {
    return wdgimage->SetImage(qimage);
}

void VHWidgetLeftPanel::Init(const QString & txtLabel, const QString & txtButton) {

    lbl = new QLabel(txtLabel, this);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    wdgimage = new VHWidgetCamImage();

    btn = new QPushButton(txtButton, this);
    connect(btn, &QPushButton::clicked, this, &VHWidgetLeftPanel::onclick);

}

void VHWidgetLeftPanel::SetupLayout() {

    auto play = new QVBoxLayout();
    play->setContentsMargins(16, 16, 16, 16);
    play->addWidget(wdgimage);

    auto vlay = new QVBoxLayout(this);
    vlay->addWidget(lbl, Qt::AlignTop);
    vlay->addLayout(play);
    vlay->addWidget(btn);

}
