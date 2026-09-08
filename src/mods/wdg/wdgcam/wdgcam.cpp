#include "wdgcam/wdgcam.hpp"

/**
 *
 */
void WidgetCamera::Init()
{
    //
    btnCameraStart = new QPushButton("Camera Start", this);

    //
    btnCameraStop = new QPushButton("Camera Stop", this);

    //
    cmbCameraList = new QComboBox();

    auto camlist = QMediaDevices::videoInputs();
    for(int i=0;i<camlist.size(); i++) {
        const QCameraDevice & cam = camlist[i];
        cmbCameraList->addItem(cam.description());
    }
    
    // QStringList distros = {"Debian", "Xubuntu", "Fedora", "Arch", "Gentoo"};
    // cmbCameraList->addItems(distros);

    //
    InitLayout();
}

/**
 *
 */
void WidgetCamera::InitLayout()
{
    // Cameray H-Layout
    auto hlay_cam = new QHBoxLayout();
    hlay_cam->addWidget(btnCameraStart);
    hlay_cam->addWidget(btnCameraStop);

    auto vlay_cam = new QVBoxLayout(this);
    vlay_cam->addLayout(hlay_cam);
    vlay_cam->addWidget(cmbCameraList);

}

/**
 *
 */
void WidgetCamera::StartCamera()
{
    camera.Init();
}

/**
 *
 */
void WidgetCamera::StopCamera()
{
}