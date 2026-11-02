#include "wdgcam/wdgcam.hpp"
#include <QTimer>
#include <QThread>

#include "vhfile.hpp"
#include "vhfsys.hpp"

/**
 * @brief Конструктор класса
 * @param parent Родительский виджет (обычно nullptr)
 */
VHWidgetCamera::VHWidgetCamera(QWidget *parent)
{
    this->parent = parent;
    Init();
}

/**
 * @brief Деструктор класса
 * Останавливает камеру и освобождает захваченные ресурсы
 */
VHWidgetCamera::~VHWidgetCamera()
{
    Deinit();
}

/**
 *
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
 * Инициализация камеры по идентификатору
 * @param id Индекс камеры в списке доступных камер
 * @return true если инициализация успешна, false в случае ошибки
 */
bool VHWidgetCamera::Init(int id)
{
    // Очищаем предыдущие ресурсы перед новой инициализацией
    Deinit();

    // Проверка валидности индекса
    if (id < 0 || id >= camerasList.size())
    {
        qDebug() << "Invalid camera id:" << id << "(max:" << camerasList.size() - 1 << ")";
        return false;
    }

    // Берём выбранную камеру
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
 * Очистка ресурсов камеры
 * Останавливает камеру, отключает сигнатуры и освобождает память
 * @return true всегда
 */
bool VHWidgetCamera::Deinit()
{
    if (!camera)
    {
        return true;
    }

    // Останавливаем камеру
    camera->stop();

    // Отключаем сигнатуру sink -> ProcessFrame
    if (sink)
    {
        QObject::disconnect(
            sink,
            &QVideoSink::videoFrameChanged,
            this,
            &VHWidgetCamera::ProcessFrame);

        // Сессия не должна хранить указатель на удаляемый sink
        session.setVideoOutput(nullptr);

        delete sink;
        sink = nullptr;
    }

    // Уничтожаем камеру
    delete camera;
    camera = nullptr;

    // Очищаем сессию
    session.setCamera(nullptr);

    return true;
}

/**
 * @brief Выбор формата видеопотока
 * Назначает выбранной камере последний (максимальный) из поддерживаемых форматов
 * @param id Индекс камеры в списке доступных камер
 */
void VHWidgetCamera::SelectFormat(int id)
{
    // Проверка валидности индекса
    if (id < 0 || id >= camerasList.size())
    {
        qDebug() << "SelectFormat(): invalid camera id:" << id;
        return;
    }

    // Список форматов выбранной камеры
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

    // Выбираем последний (максимальный) формат
    camera->setCameraFormat(formats.last());
}

/**
 * @brief Запуск камеры
 * Возобновляет поток кадров уже инициализированной камеры
 */
void VHWidgetCamera::StartCamera()
{
    if (!camera)
    {
        qDebug() << "StartCamera(): camera is not initialized";
        return;
    }

    camera->start();
}

/**
 * @brief Остановка камеры
 * Приостанавливает поток кадров, ресурсы камеры сохраняются
 */
void VHWidgetCamera::StopCamera()
{
    if (!camera)
    {
        return;
    }

    camera->stop();
}

/**
 *
 */
void VHWidgetCamera::ProcessFrame(const QVideoFrame &frame)
{

    if (!frame.isValid())
        return;

    // Inform parent
    QImage img = frame.toImage();
    if (!img.isNull())
    {
        emit frameReady(img);
        wdgImage->setimage(img);
    }

    // Frame info
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

    if (!firstsnap)
    {
        firstsnap = true;
        VHFile fileout("yuvv.bin");
        fileout.write(data, size);
    }

    qDebug() << "YUV буфер:" << size << "байт, stride:" << stride;
    frame2.unmap();
}

/**
 *
 */
void VHWidgetCamera::Init()
{
    //
    InitObjects();

    //
    InitLayout();
}

/**
 *
 */
void VHWidgetCamera::InitObjects()
{

    // Available Camera List
    cmbCameraList = new QComboBox();

    int camscount = Enumerate();

    auto camlist = QMediaDevices::videoInputs();
    for (int i = 0; i < camlist.size(); i++)
    {
        const QCameraDevice &cam = camlist[i];
        cmbCameraList->addItem(cam.description());
    }

    // Available Resolution List
    cmbResolutionsList = new QComboBox();

    //
    wdgImage = new WidgetImage();
    wdgImage->setBaseSize(640, 480);

    //
    btnCameraStart = new QPushButton("Camera Start", this);
    connect(btnCameraStart, &QPushButton::clicked, this, &VHWidgetCamera::OnClickCameraStart);

    //
    btnCameraStop = new QPushButton("Camera Stop", this);
    connect(btnCameraStop, &QPushButton::clicked, this, &VHWidgetCamera::OnClickCameraStop);
}

/**
 *
 */
void VHWidgetCamera::InitLayout()
{
    // Cameray H-Layout
    auto hlay_cam = new QHBoxLayout();
    hlay_cam->addWidget(btnCameraStart);
    hlay_cam->addWidget(btnCameraStop);

    auto vlay_cam = new QVBoxLayout(this);
    vlay_cam->addWidget(cmbCameraList);
    vlay_cam->addWidget(cmbResolutionsList);
    vlay_cam->addWidget(wdgImage);
    vlay_cam->addLayout(hlay_cam);
}

/**
 * @brief Обработчик кнопки "Camera Start"
 * При необходимости инициализирует выбранную камеру и запускает поток кадров
 */
void VHWidgetCamera::OnClickCameraStart()
{
    int idx = cmbCameraList->currentIndex();

    // Камера ещё не создана - создаём её по выбранному индексу
    if (!camera)
    {
        if (!Init(idx))
            return;
    }

    StartCamera();
}

/**
 * @brief Обработчик кнопки "Camera Stop"
 * Приостанавливает поток кадров, ресурсы камеры сохраняются
 */
void VHWidgetCamera::OnClickCameraStop()
{
    StopCamera();
}

// QCameraFormat fmt = cameras.first().videoFormats().first();
// camera->setCameraFormat(fmt);
// qDebug() << "Using format:" << fmt.resolution() << fmt.pixelFormat();
