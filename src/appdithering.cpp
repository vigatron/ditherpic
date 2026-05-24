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

#include <QApplication>
#include <QMainWindow>

#include "wdgDitheringWnd.hpp"

/*

WidgetDitheringWnd

-------------------------------------------------------
|                   |               |                 |
| OriginalPic       | Settings      | ResultPic       |
|                   |               |                 |
-------------------------------------------------------

*/

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    WidgetDitheringWnd *w = new WidgetDitheringWnd();
    w->setWindowTitle("VHPIC Images Converter / V01G04A81 2026");
    // w->resize(1400, 600);
    w->show();

    return app.exec();
}
