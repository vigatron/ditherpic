#include "camera.hpp"
#include <QTimer>
#include <QThread>

/**
 *
 */
int VHCameraObject::Init()
{
    // Список камер
    camerasList = QMediaDevices::videoInputs();

    for (auto &item : camerasList)
    {
        qDebug() << "Camera: "
                 << item.description() << "\n";
    }

    // Берём первую камеру
    camera = new QCamera(camerasList.first());
    session.setCamera(camera);

    sink = new QVideoSink;
    session.setVideoOutput(sink);

    QObject::connect(
        sink,
        &QVideoSink::videoFrameChanged,
        this,
        &VHCameraObject::ProcessFrame);

    // QCameraFormat fmt = cameras.first().videoFormats().first();
    // camera->setCameraFormat(fmt);
    // qDebug() << "Using format:" << fmt.resolution() << fmt.pixelFormat();

    SelectFormat();

    return camerasList.size();
}

/**
 *
 */
void VHCameraObject::SelectFormat()
{
    // Set Format
    for (auto &fmt : camerasList.first().videoFormats())
    {
        qDebug() << "Format:" << fmt.resolution() << fmt.pixelFormat();
    }
    camera->setCameraFormat(camerasList.first().videoFormats().last());

}

/**
 *
 */
void VHCameraObject::StartCamera()
{
    // QThread::sleep(2);

    this->camera->start();

}

/**
 *
 */
void VHCameraObject::ProcessFrame(const QVideoFrame &frame)
{

    if (!frame.isValid())
        return;

    qDebug() << "Кадр:"
             << frame.width()
             << "x"
             << frame.height()
             << "формат:"
             << frame.pixelFormat();

    QVideoFrame f(frame);

    if (f.map(QVideoFrame::ReadOnly))
    {
        const uchar *data = f.bits(0);
        int stride = f.bytesPerLine(0);
        int size = f.mappedBytes(0);

        qDebug() << "YUV буфер:" << size << "байт, stride:" << stride;
        f.unmap();
    }
    else
    {
        qDebug() << "ProcessFrame() exception";
    }
}