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

#include <QCamera>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QVideoFrame>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include <QHeaderView>
#include <QTableWidget>
#include <QStringList>
#include <QComboBox>

#include "vhqt.hpp"
#include "vhfile.hpp"
#include "vhfsys.hpp"

#include "wdgimage/wdgimage.hpp"

/**
 * @brief Camera widget
 *
 * Combines the camera capture layer (device enumeration, capture session,
 * video sink and frame processing) with the user interface used to preview
 * incoming frames and to control the capture process.
 */
class VHWidgetCamera : public QWidget
{

    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param parent Parent widget (usually nullptr)
     */
    explicit VHWidgetCamera(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     * Closes the camera and releases all captured resources
     */
    ~VHWidgetCamera();

    /**
     * @brief Enumerate available cameras
     * Fills the internal camera list and prints device descriptions to debug output
     * @return Number of available cameras
     */
    int Enumerate();

    /**
     * @brief Get the number of available cameras
     * @return Number of cameras in the internal list
     */
    int CameraCount() const { return camerasList.size(); }

    /**
     * @brief Open a camera by its index
     * Closes the previously opened camera, creates QCamera and QVideoSink,
     * connects frame processing and applies the video format
     * @param id Index of the camera in the available cameras list
     * @return true on success, false on error
     */
    bool CameraOpen(int id);

    /**
     * @brief Close the camera
     * Stops the camera, disconnects all signals and releases allocated memory
     * @return true always
     */
    bool CameraClose();

    /**
     * @brief Get a reference to the camera device by its index
     * @param n Index of the camera in the available cameras list
     * @return Reference to the camera device
     */
    const QCameraDevice &GetCamera(int n) const { return camerasList[n]; }

signals:

    /**
     * @brief Frame ready signal
     * Emitted from ProcessFrame() when a new video frame has been decoded
     * @param img Image of the captured frame
     */
    void frameReady(const QImage &img);

private slots:

    // Handler of the "Camera Start" button
    void OnClickCameraStart();

    // Handler of the "Camera Stop" button
    void OnClickCameraStop();

private:

    // ------------------------------------------------------------------
    // Camera capture layer
    // ------------------------------------------------------------------

    QCamera *camera = nullptr;
    QList<QCameraDevice> camerasList;
    QMediaCaptureSession session;
    QVideoSink *sink = nullptr;

    // Set to true after the first frame has been dumped to a debug file
    bool firstSnap = false;

    // ------------------------------------------------------------------
    // Widget user interface
    // ------------------------------------------------------------------

    QPushButton *btnCameraStart;
    QPushButton *btnCameraStop;

    // Available cameras list
    QComboBox *cmbCameraList;

    // Supported resolutions list
    QComboBox *cmbResolutionsList;

    // Frame preview area
    WidgetImage *wdgImage;

    // ------------------------------------------------------------------
    // Camera capture methods
    // ------------------------------------------------------------------

    /**
     * @brief Select the video format
     * Assigns the last (maximum) supported format to the selected camera
     * @param id Index of the camera in the available cameras list
     */
    void SelectFormat(int id);

    /**
     * @brief Start the camera
     * Resumes the frame stream of an already opened camera
     */
    void CameraStart();

    /**
     * @brief Stop the camera
     * Suspends the frame stream, camera resources are kept alive
     */
    void CameraStop();

    /**
     * @brief Process a video frame
     * Converts the frame to an image, notifies listeners and
     * updates the preview area
     * @param frame Video frame received from the capture session
     */
    void ProcessFrame(const QVideoFrame &frame);

    // ------------------------------------------------------------------
    // Widget user interface methods
    // ------------------------------------------------------------------

    /**
     * @brief Initialize the widget
     * Creates the child widgets and places them into the layout
     */
    void UiInit();

    /**
     * @brief Create the child widgets
     * Creates the camera and resolution lists, the frame preview area
     * and the capture control buttons
     */
    void UiInitObjects();

    /**
     * @brief Arrange the child widgets
     * Builds the widget layout
     */
    void UiInitLayout();
};
