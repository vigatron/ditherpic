#pragma once

#include "vhqt.hpp"

#include <QHeaderView>
#include <QTableWidget>
#include <QStringList>
#include <QComboBox>

#include "camera/camera.hpp"

class WidgetCamera : public QWidget
{

    Q_OBJECT

public:
    explicit WidgetCamera(QWidget *parent = nullptr);

signals:

    // emitted in ProcessFrame
    void frameReady(const QImage &img);

private slots:

    // Event from button
    void OnClickCameraStart();

    // Event from button
    void OnClickCameraStop();

    // Event from VHCameraObject
    void onFrameReady(const QImage &img);

private:

    QWidget *parent;

    // camera object
    VHCameraObject camera;

    //
    QPushButton *btnCameraStart;
    QPushButton *btnCameraStop;
    QComboBox *cmbCameraList;

    //
    void Init();

    //
    void InitObjects();

    //
    void InitLayout();

    //
    void StartCamera();

    //
    void StopCamera();

    // auto vlayleft = new QVBoxLayout();
    // vlayleft->addWidget(btnCameraStart);
};
