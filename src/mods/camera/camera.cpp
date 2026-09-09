#include "camera.hpp"
#include <QTimer>
#include <QThread>


VHCameraObject::VHCameraObject(QObject *parent)
{

}

/**
 *
 */
int VHCameraObject::Enumerate()
{
    camerasList = QMediaDevices::videoInputs();

    for (auto &item : camerasList)
    {
        qDebug() << "Camera: " << item.description();
    }

    return camerasList.size();
}

/**
 *
 */
bool VHCameraObject::Init(int id)
{

    // Берём первую камеру
    camera = new QCamera(camerasList[id]);
    session.setCamera(camera);

    sink = new QVideoSink;
    session.setVideoOutput(sink);

    QObject::connect(
        sink,
        &QVideoSink::videoFrameChanged,
        this,
        &VHCameraObject::ProcessFrame);

    // SelectFormat();

    return true;
}

/**
 * 
 */
bool VHCameraObject::Deinit() {

    return true;
}


/**
 * Set Format
 */
void VHCameraObject::SelectFormat()
{
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
    this->camera->start();
}

/**
 *
 */
void VHCameraObject::StopCamera() {
    this->camera->stop();
}

/**
 *
 */
void VHCameraObject::ProcessFrame(const QVideoFrame &frame)
{

    if (!frame.isValid())
        return;

    // Inform parent
    QImage img = frame.toImage();
    if (!img.isNull()) {
        emit frameReady(img);
    }

    // Frame info
    qDebug() << "Кадр:"
             << frame.width()
             << "x"
             << frame.height()
             << "формат:"
             << frame.pixelFormat();

    QVideoFrame frame2(frame);

    if (frame2.map(QVideoFrame::ReadOnly))
    {
        const uchar *data = frame2.bits(0);
        int stride = frame2.bytesPerLine(0);
        int size = frame2.mappedBytes(0);

        qDebug() << "YUV буфер:" << size << "байт, stride:" << stride;
        frame2.unmap();
    }
    else
    {
        qDebug() << "ProcessFrame() exception";
    }
}

// QCameraFormat fmt = cameras.first().videoFormats().first();
// camera->setCameraFormat(fmt);
// qDebug() << "Using format:" << fmt.resolution() << fmt.pixelFormat();
