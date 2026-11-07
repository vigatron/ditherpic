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

#include <QApplication>
#include <QMainWindow>

#include "wdgDitheringWnd.hpp"

/**
 * @brief Точка входа приложения
 * @param argc Количество аргументов командной строки
 * @param argv Аргументы командной строки
 * @return Код возврата приложения
 */
int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Объект создаётся на стеке: деструктор окна должен выполняться,
    // он закрывает камеру и освобождает устройство захвата
    WidgetDitheringWnd w;
    w.setWindowTitle("VHPIC Images Converter / V01G04A81 2026");
    w.show();

    return app.exec();
}
