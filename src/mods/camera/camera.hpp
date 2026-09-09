#pragma once

#include <QCamera>
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMediaCaptureSession>
#include <QVideoSink>
#include <QVideoFrame>
#include <QDebug>

class VHCameraObject : public QObject
{
Q_OBJECT // обязательно для сигналов/слотов

    public : explicit VHCameraObject(QObject *parent = nullptr);

    //
    int Enumerate();

    //
    int camscount() { return camerasList.size(); }

    //
    bool Init(int id);

    //
    bool Deinit();

    //
    const QCameraDevice &GetCamera(int n) { return camerasList[n]; }

signals:

    // emitted in ProcessFrame
    void frameReady(const QImage &img);

private:
    QCamera *camera;
    QList<QCameraDevice> camerasList;
    QMediaCaptureSession session;
    QVideoSink *sink;

    //
    void SelectFormat();

    //
    void StartCamera();

    //
    void StopCamera();

    //
    void ProcessFrame(const QVideoFrame &frame);
};