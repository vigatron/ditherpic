#include "wdgformat.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

WdgFormat::WdgFormat(QWidget *parent)
    : QWidget(parent)
{
    initFormats();
    initButtons();
}

void WdgFormat::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
}

void WdgFormat::initFormats()
{
    m_formats << qMakePair(QSize(1280, 720), "Format_Jpeg")
              << qMakePair(QSize(160, 120), "Format_Jpeg")
              << qMakePair(QSize(176, 144), "Format_Jpeg")
              << qMakePair(QSize(320, 240), "Format_Jpeg")
              << qMakePair(QSize(352, 288), "Format_Jpeg")
              << qMakePair(QSize(640, 480), "Format_Jpeg")
              << qMakePair(QSize(1280, 720), "Format_YUYV")
              << qMakePair(QSize(160, 120), "Format_YUYV")
              << qMakePair(QSize(176, 144), "Format_YUYV")
              << qMakePair(QSize(320, 240), "Format_YUYV")
              << qMakePair(QSize(352, 288), "Format_YUYV")
              << qMakePair(QSize(640, 480), "Format_YUYV");
}

void WdgFormat::initButtons()
{
    m_listWidget = new QListWidget(this);
    m_selectButton = new QPushButton("Select Format", this);
    m_closeButton = new QPushButton("Сlose", this);

    // Connect signals
    connect(m_listWidget, &QListWidget::currentRowChanged, this, &WdgFormat::onFormatSelected);
    connect(m_closeButton, &QPushButton::clicked, this, &WdgFormat::onCloseClicked);

    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_listWidget);
    mainLayout->addWidget(m_selectButton);
    mainLayout->addWidget(m_closeButton);

    // Show the widget
    show();
}

void WdgFormat::selectFormat(int index)
{
    if (index >= 0 && index < static_cast<int>(m_formats.size())) {
        // Logic to select format at given index
        qDebug() << "Selected format at index" << index;
    }
}

void WdgFormat::onFormatSelected(int index)
{
    // Handle format selection
    qDebug() << "Format selected at index:" << index;
}

void WdgFormat::onCloseClicked()
{
    // Close the window
    close();
}
