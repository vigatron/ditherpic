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
    explicit WidgetCamera(QWidget *parent = nullptr) { Init(); }

private slots:

    void OnClickCameraStart();
    void OnClickCameraStop();

private:
    // camera object
    VHCameraObject camera;

    //
    QPushButton *btnCameraStart;
    QPushButton *btnCameraStop;
    QComboBox *cmbCameraList;

    //
    void Init();

    //
    void InitLayout();

    //
    void StartCamera();

    //
    void StopCamera();

    // auto vlayleft = new QVBoxLayout();
    // vlayleft->addWidget(btnCameraStart);
};
