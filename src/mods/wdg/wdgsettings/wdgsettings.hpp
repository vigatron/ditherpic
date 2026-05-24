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

#pragma once

#include "vhqt.hpp"

#include <QHeaderView>
#include <QTableWidget>
#include <QStringList>

class WidgetSettings : public QWidget {

    Q_OBJECT

    public:

        explicit WidgetSettings(QWidget * parent=nullptr);

        const int   idxflt()    const     { return listWidgetFilters.currentRow();  }
        const int   idxpal()    const     { return listWidgetPalettes.currentRow(); }
        const int   idxreso()   const     { return tblWidgetModes.currentRow();    }

    signals:

        void StartConversion();

    private slots:

        void OnConvert();

    private:

        // Mid part
        QGroupBox   *   grpFilters;
        QGroupBox   *   grpColors;
        QGroupBox   *   grpModes;

        QPushButton *   btnConvert;

        QListWidget     listWidgetFilters;
        QListWidget     listWidgetPalettes;
        QTableWidget    tblWidgetModes;

        QVBoxLayout *   InitLayout();

        void            InitFilters();
        void            InitColors();
        void            InitModes();


};
