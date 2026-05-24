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

#include "wdgsettings.hpp"

#include "tblfilters.hpp"
#include "tblreso.hpp"
#include "tblcolors.hpp"


WidgetSettings::WidgetSettings(QWidget * parent) : QWidget(parent) {
    setLayout(InitLayout());
}

void WidgetSettings::OnConvert() {
    emit StartConversion();
}

QVBoxLayout * WidgetSettings::InitLayout() {

    setMinimumWidth(320);
    setMaximumWidth(360);

    btnConvert      = new QPushButton("Convert", this);
    connect(btnConvert, &QPushButton::clicked, this, &WidgetSettings::OnConvert);

    InitFilters();
    InitColors();
    InitModes();

    // Layout для GroupBoxFilters
    QVBoxLayout *layoutFilters = new QVBoxLayout;
    layoutFilters->addWidget(&listWidgetFilters);
    grpFilters->setLayout(layoutFilters);

    // Layout для GroupBoxPalettes
    QVBoxLayout *layoutPalettes = new QVBoxLayout;
    layoutPalettes->addWidget(&listWidgetPalettes);
    grpColors->setLayout(layoutPalettes);

    grpFilters->layout()->setContentsMargins(0, 0, 0, 0);
    grpFilters->layout()->setSpacing(0);
    grpFilters->setFlat(true);

    grpColors->layout()->setContentsMargins(0, 0, 0, 0);
    grpColors->layout()->setSpacing(0);
    grpColors->setFlat(true);

    auto hlay = new QHBoxLayout();
    hlay->addWidget(grpFilters, 3);
    hlay->addWidget(grpColors , 2);

    auto vlay = new QVBoxLayout(this);
    vlay->addLayout(hlay, 2);
    vlay->addWidget(grpModes, 4);
    vlay->addWidget(btnConvert);

    return vlay;
}

void WidgetSettings::InitFilters() {

    grpFilters  = new QGroupBox("Filters", this);

    for(int i=0;i<VHDither::count;i++) {
        std::string_view str = VHDither::tblFilters[i].name;
        listWidgetFilters.addItem(
            QString::fromUtf8(str.data(), str.size()) ); }

    listWidgetFilters.setCurrentRow(0);
}

void WidgetSettings::InitColors() {

    grpColors   = new QGroupBox("Colors", this);

    for(int i=0;i<VHColors::count;i++) {
        std::string_view str = VHColors::names[i];
        listWidgetPalettes.addItem(
            QString::fromUtf8(str.data(), str.size())); }

    listWidgetPalettes.setCurrentRow(0);
}

void WidgetSettings::InitModes() {

    grpModes = new QGroupBox("Video mode", this);

    tblWidgetModes.setColumnCount(3);
    tblWidgetModes.setRowCount(VHReso::count);

    tblWidgetModes.horizontalHeader()->setVisible(false);
    tblWidgetModes.verticalHeader()->setVisible(false);

    for(int i=0; i < VHReso::count; i++) {

        std::string_view strmode = VHReso::tblResolutions[i].name;
        std::string_view straspc = VHReso::tblResolutions[i].aspc;

        QString txtmode     = QString::fromUtf8(strmode.data(), strmode.size());

        QString txtw        = QString::number(VHReso::tblResolutions[i].w);
        QString txth        = QString::number(VHReso::tblResolutions[i].h);
        QString txtreso     = i ? (txtw + ":" + txth) : "";

        QString txta        = QString::fromUtf8(straspc.data(), straspc.size());
        QString txtaspc     = i ? ("(" + txta + ")") : "";

        QTableWidgetItem *item0 = new QTableWidgetItem(txtmode);
        // item0->setTextAlignment(Qt::AlignLeft);
        tblWidgetModes.setItem(i, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(txtreso);
        item1->setTextAlignment(Qt::AlignCenter);
        tblWidgetModes.setItem(i, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(txtaspc);
        item2->setTextAlignment(Qt::AlignCenter);
        tblWidgetModes.setItem(i, 2, item2);

    }

    tblWidgetModes.setCurrentCell(0,0);
    tblWidgetModes.horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // tblWidgetModes.horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    tblWidgetModes.resizeRowsToContents();
    tblWidgetModes.show();

    // Layout для GroupBox
    QVBoxLayout * layoutModes = new QVBoxLayout;
    layoutModes->addWidget(&tblWidgetModes);
    grpModes->setLayout(layoutModes);

}
