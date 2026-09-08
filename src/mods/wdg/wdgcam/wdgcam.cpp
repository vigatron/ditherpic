#include "wdgcam/wdgcam.hpp"

/**
 *
 */
void WidgetCamera::Init()
{
    //
    btnCameraStart = new QPushButton("Camera Start", this);
    connect(btnCameraStart, &QPushButton::clicked, this, &WidgetCamera::OnClickCameraStart);

    //
    btnCameraStop = new QPushButton("Camera Stop", this);
    connect(btnCameraStart, &QPushButton::clicked, this, &WidgetCamera::OnClickCameraStop);

    //
    cmbCameraList = new QComboBox();

    int camscount = camera.Enumerate();

    auto camlist = QMediaDevices::videoInputs();
    for(int i=0;i<camlist.size(); i++) {
        const QCameraDevice & cam = camlist[i];
        cmbCameraList->addItem(cam.description());
    }

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
}

/**
 *
 */
void WidgetCamera::StopCamera()
{
}

/**
 *
 */
void WidgetCamera::OnClickCameraStart()
{
    int idx = cmbCameraList->currentIndex();
    camera.Init(idx);
}

/**
 *
 */
void WidgetCamera::OnClickCameraStop()
{
    camera.Deinit();
}
