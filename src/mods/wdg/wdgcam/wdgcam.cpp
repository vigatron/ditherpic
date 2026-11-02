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

#include "wdgcam/wdgcam.hpp"
#include <QTimer>
#include <QThread>

#include "vhfile.hpp"
#include "vhfsys.hpp"

// ---------------------------------------------------------------------------
// Camera capture layer
// ---------------------------------------------------------------------------

/**
 * @brief Constructor
 * @param parent Parent widget (usually nullptr)
 */
VHWidgetCamera::VHWidgetCamera(QWidget *parent) : QWidget(parent)
{
    UiInit();
}

/**
 * @brief Destructor
 * Closes the camera and releases all captured resources
 */
VHWidgetCamera::~VHWidgetCamera()
{
    CameraClose();
}

/**
 * @brief Enumerate available cameras
 * Fills the internal camera list and prints device descriptions to debug output
 * @return Number of available cameras
 */
int VHWidgetCamera::Enumerate()
{
    camerasList = QMediaDevices::videoInputs();

    for (auto &item : camerasList)
    {
        qDebug() << "Camera: " << item.description();
    }

    return camerasList.size();
}

/**
 * @brief Open a camera by its index
 * Closes the previously opened camera, creates QCamera and QVideoSink,
 * connects frame processing and applies the video format
 * @param id Index of the camera in the available cameras list
 * @return true on success, false on error
 */
bool VHWidgetCamera::CameraOpen(int id)
{
    // Release previous resources before opening a new camera
    CameraClose();

    // Validate the camera index
    if (id < 0 || id >= camerasList.size())
    {
        qDebug() << "CameraOpen(): invalid camera id:" << id
                 << "(max:" << camerasList.size() - 1 << ")";
        return false;
    }

    // Create the selected camera
    camera = new QCamera(camerasList[id]);
    session.setCamera(camera);

    sink = new QVideoSink;
    session.setVideoOutput(sink);

    QObject::connect(
        sink,
        &QVideoSink::videoFrameChanged,
        this,
        &VHWidgetCamera::ProcessFrame);

    SelectFormat(id);

    return true;
}

/**
 * @brief Close the camera
 * Stops the camera, disconnects all signals and releases allocated memory
 * @return true always
 */
bool VHWidgetCamera::CameraClose()
{
    if (!camera)
    {
        return true;
    }

    // Stop the camera
    camera->stop();

    // Disconnect sink -> ProcessFrame
    if (sink)
    {
        QObject::disconnect(
            sink,
            &QVideoSink::videoFrameChanged,
            this,
            &VHWidgetCamera::ProcessFrame);

        // The session must not keep a pointer to the sink being deleted
        session.setVideoOutput(nullptr);

        delete sink;
        sink = nullptr;
    }

    // Destroy the camera
    delete camera;
    camera = nullptr;

    // Clear the capture session
    session.setCamera(nullptr);

    return true;
}

/**
 * @brief Select the video format
 * Assigns the last (maximum) supported format to the selected camera
 * @param id Index of the camera in the available cameras list
 */
void VHWidgetCamera::SelectFormat(int id)
{
    // Validate the camera index
    if (id < 0 || id >= camerasList.size())
    {
        qDebug() << "SelectFormat(): invalid camera id:" << id;
        return;
    }

    // Formats supported by the selected camera
    const QList<QCameraFormat> formats = camerasList[id].videoFormats();
    if (formats.isEmpty())
    {
        qDebug() << "SelectFormat(): no video formats for camera id:" << id;
        return;
    }

    for (const auto &fmt : formats)
    {
        qDebug() << "Format:" << fmt.resolution() << fmt.pixelFormat();
    }

    // Apply the last (maximum) format
    camera->setCameraFormat(formats.last());
}

/**
 * @brief Start the camera
 * Resumes the frame stream of an already opened camera
 */
void VHWidgetCamera::CameraStart()
{
    if (!camera)
    {
        qDebug() << "CameraStart(): camera is not opened";
        return;
    }

    camera->start();
}

/**
 * @brief Stop the camera
 * Suspends the frame stream, camera resources are kept alive
 */
void VHWidgetCamera::CameraStop()
{
    if (!camera)
    {
        return;
    }

    camera->stop();
}

/**
 * @brief Process a video frame
 * Converts the frame to an image, notifies listeners and
 * updates the preview area
 * @param frame Video frame received from the capture session
 */
void VHWidgetCamera::ProcessFrame(const QVideoFrame &frame)
{

    if (!frame.isValid())
        return;

    // Notify listeners and update the preview area
    QImage img = frame.toImage();
    if (!img.isNull())
    {
        emit frameReady(img);
        wdgImage->setimage(img);
    }

    // Frame debug information
    qDebug() << "Кадр:"
             << frame.width()
             << "x"
             << frame.height()
             << "формат:"
             << frame.pixelFormat();

    QVideoFrame frame2(frame);

    if (!frame2.map(QVideoFrame::ReadOnly))
    {
        qDebug() << "ProcessFrame() exception";
        return;
    }

    const uchar *data = frame2.bits(0);
    int stride = frame2.bytesPerLine(0);
    int size = frame2.mappedBytes(0);

    // Dump the first frame to a file for further analysis
    if (!firstSnap)
    {
        firstSnap = true;
        VHFile fileout("yuvv.bin");
        fileout.write(data, size);
    }

    qDebug() << "YUV буфер:" << size << "байт, stride:" << stride;
    frame2.unmap();
}

// ---------------------------------------------------------------------------
// Widget user interface
// ---------------------------------------------------------------------------

/**
 * @brief Initialize the widget
 * Creates the child widgets and places them into the layout
 */
void VHWidgetCamera::UiInit()
{
    UiInitObjects();

    UiInitLayout();
}

/**
 * @brief Create the child widgets
 * Creates the camera and resolution lists, the frame preview area
 * and the capture control buttons
 */
void VHWidgetCamera::UiInitObjects()
{

    // Available cameras list
    cmbCameraList = new QComboBox();

    int camscount = Enumerate();

    auto camlist = QMediaDevices::videoInputs();
    for (int i = 0; i < camlist.size(); i++)
    {
        const QCameraDevice &cam = camlist[i];
        cmbCameraList->addItem(cam.description());
    }

    // Supported resolutions list
    cmbResolutionsList = new QComboBox();

    // Frame preview area
    wdgImage = new WidgetImage();
    wdgImage->setBaseSize(640, 480);

    // Capture control buttons
    btnCameraStart = new QPushButton("Camera Start", this);
    connect(btnCameraStart, &QPushButton::clicked, this, &VHWidgetCamera::OnClickCameraStart);

    btnCameraStop = new QPushButton("Camera Stop", this);
    connect(btnCameraStop, &QPushButton::clicked, this, &VHWidgetCamera::OnClickCameraStop);
}

/**
 * @brief Arrange the child widgets
 * Builds the widget layout
 */
void VHWidgetCamera::UiInitLayout()
{
    // Capture control buttons layout
    auto hlayButtons = new QHBoxLayout();
    hlayButtons->addWidget(btnCameraStart);
    hlayButtons->addWidget(btnCameraStop);

    // Main widget layout
    auto vlayMain = new QVBoxLayout(this);
    vlayMain->addWidget(cmbCameraList);
    vlayMain->addWidget(cmbResolutionsList);
    vlayMain->addWidget(wdgImage);
    vlayMain->addLayout(hlayButtons);
}

/**
 * @brief Handler of the "Camera Start" button
 * Opens the selected camera if needed and starts the frame stream
 */
void VHWidgetCamera::OnClickCameraStart()
{
    int idx = cmbCameraList->currentIndex();

    // Open the camera by the selected index if it is not opened yet
    if (!camera)
    {
        if (!CameraOpen(idx))
            return;
    }

    CameraStart();
}

/**
 * @brief Handler of the "Camera Stop" button
 * Suspends the frame stream, camera resources are kept alive
 */
void VHWidgetCamera::OnClickCameraStop()
{
    CameraStop();
}
