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

#include <QFrame>

class HLineSep : public QFrame {
public:
  HLineSep() {
    setMinimumHeight(4);
    setFrameShape(QFrame::HLine);
    setFrameShadow(QFrame::Sunken);
  }
};

class VLineSep : public QFrame {
public:
  VLineSep() {
    setMinimumWidth(4);
    setFrameShape(QFrame::VLine);
    setFrameShadow(QFrame::Sunken);
  }
};
