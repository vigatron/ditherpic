#include "wdgcam/wdgcam.hpp"

/**
 *
 */
WidgetCamera::WidgetCamera(QWidget *parent)
{
    this->parent = parent;
    Init();
}

/**
 *
 */
void WidgetCamera::Init()
{
    //
    InitObjects();

    //
    InitLayout();
}

/**
 *
 */
void WidgetCamera::InitObjects()
{
    //
    btnCameraStart = new QPushButton("Camera Start", this);
    connect(btnCameraStart, &QPushButton::clicked, this, &WidgetCamera::OnClickCameraStart);

    //
    btnCameraStop = new QPushButton("Camera Stop", this);
    connect(btnCameraStop, &QPushButton::clicked, this, &WidgetCamera::OnClickCameraStop);

    //
    cmbCameraList = new QComboBox();

    int camscount = camera.Enumerate();

    auto camlist = QMediaDevices::videoInputs();
    for (int i = 0; i < camlist.size(); i++)
    {
        const QCameraDevice &cam = camlist[i];
        cmbCameraList->addItem(cam.description());
    }

    QObject::connect(
        &camera,
        &VHCameraObject::frameReady,
        this,
        &WidgetCamera::onFrameReady);
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
    vlay_cam->addWidget(cmbCameraList);
    vlay_cam->addLayout(hlay_cam);
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

/**
 *
 */
void WidgetCamera::onFrameReady(const QImage &img)
{
    emit frameReady(img);
}
