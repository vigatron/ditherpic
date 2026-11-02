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

    // There is nothing to open without a device
    if (camerasList.isEmpty())
    {
        ReportError("No cameras available");
        return false;
    }

    // Validate the camera index
    if (id < 0 || id >= camerasList.size())
    {
        ReportError(
            QString("Invalid camera index: %1 (max: %2)")
                .arg(id)
                .arg(camerasList.size() - 1));
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

    // Forward hardware level errors to the listeners
    QObject::connect(
        camera,
        &QCamera::errorOccurred,
        this,
        [this](QCamera::Error err, const QString &txt)
        {
            if (err != QCamera::NoError)
                ReportError(txt);
        });

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
    cameraRunning = false;

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
 * @brief Start the camera
 * Resumes the frame stream of an already opened camera
 */
void VHWidgetCamera::CameraStart()
{
    if (!camera)
    {
        ReportError("Camera is not opened");
        return;
    }

    camera->start();
    cameraRunning = true;
}

/**
 * @brief Stop the camera
 * Suspends the frame stream, camera resources are kept alive
 */
void VHWidgetCamera::CameraStop()
{
    cameraRunning = false;

    if (!camera)
    {
        return;
    }

    camera->stop();
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
        return;
    }

    // Formats supported by the selected camera
    const QList<QCameraFormat> formats = camerasList[id].videoFormats();
    if (formats.isEmpty())
    {
        ReportError("No video formats for the selected camera");
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
 * @brief Process a video frame
 * Converts the frame to an image, updates the preview area and
 * emits frameReady() for external listeners
 * @param frame Video frame received from the capture session
 */
void VHWidgetCamera::ProcessFrame(const QVideoFrame &frame)
{

    if (!frame.isValid())
        return;

    // The frame is always rendered by the internal preview area,
    // frameReady() is emitted additionally for external listeners
    QImage img = frame.toImage();
    if (!img.isNull())
    {
        wdgImage->setimage(img);
        emit frameReady(img);
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

/**
 * @brief Report an error
 * Prints the message to the log and emits errorOccurred()
 * @param msg Human readable error description
 */
void VHWidgetCamera::ReportError(const QString &msg)
{
    qWarning() << "VHWidgetCamera:" << msg;
    emit errorOccurred(msg);
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
 * Creates the camera list, the frame preview area and the control buttons
 */
void VHWidgetCamera::UiInitObjects()
{

    // Available cameras list
    cmbCameraList = new QComboBox();

    Enumerate();

    for (const QCameraDevice &cam : camerasList)
    {
        cmbCameraList->addItem(cam.description());
    }

    // Frame preview area
    wdgImage = new WidgetImage();
    wdgImage->setBaseSize(640, 480);

    // Capture control buttons
    btnCameraStart = new QPushButton("Camera Start", this);
    connect(btnCameraStart, &QPushButton::clicked, this, &VHWidgetCamera::OnClickCameraStart);

    btnCameraStop = new QPushButton("Camera Stop", this);
    connect(btnCameraStop, &QPushButton::clicked, this, &VHWidgetCamera::OnClickCameraStop);

    // Connect the camera selection handler after the list has been filled,
    // otherwise it would fire on every item insertion
    connect(
        cmbCameraList,
        &QComboBox::currentIndexChanged,
        this,
        &VHWidgetCamera::OnCameraSelected);
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

/**
 * @brief Handler of the camera selection change
 * Reopens the capture session on the newly selected camera
 * @param idx Index of the selected camera
 */
void VHWidgetCamera::OnCameraSelected(int idx)
{
    // Nothing to switch if the camera is not opened yet
    if (!camera || idx < 0)
        return;

    const bool wasRunning = cameraRunning;

    if (!CameraOpen(idx))
        return;

    if (wasRunning)
        CameraStart();
}
