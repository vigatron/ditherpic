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

class VHCameraObject : public QObject
{
    Q_OBJECT

public:
    /**
     * Конструктор класса
     * @param parent Родительский объект (обычно nullptr)
     */
    explicit VHCameraObject(QObject *parent = nullptr);

    /**
     * Деструктор класса
     */
    ~VHCameraObject();

    /**
     * Перечисление доступных камер
     * Заполняет список камер и выводит описание в отладочный вывод
     * @return Количество доступных камер
     */
    int Enumerate();

    /**
     * Получение количества доступных камер
     * @return Количество камер
     */
    int camscount() { return camerasList.size(); }

    /**
     * Инициализация камеры по идентификатору
     * @param id Индекс камеры в списке доступных камер
     * @return true если инициализация успешна, false в случае ошибки
     */
    bool Init(int id);

    /**
     * Очистка ресурсов камеры
     * Останавливает камеру, отключает сигнатуры и освобождает память
     * @return true всегда
     */
    bool Deinit();

    /**
     * Получение ссылки на камеру по индексу
     * @param n Индекс камеры
     * @return Ссылка на камеру
     */
    const QCameraDevice &GetCamera(int n) { return camerasList[n]; }

signals:
    /**
     * Сигнал готовности кадра
     * Излучается в методе ProcessFrame при получении нового кадра
     * @param img Картинка кадра
     */
    void frameReady(const QImage &img);

private:
    QCamera *camera;
    QList<QCameraDevice> camerasList;
    QMediaCaptureSession session;
    QVideoSink *sink;

    bool firstsnap = false;

    /**
     * Выбор формата видеопотока
     */
    void SelectFormat();

    /**
     * Запуск камеры
     */
    void StartCamera();

    /**
     * Остановка камеры
     */
    void StopCamera();

    /**
     * Обработка кадра видео
     * @param frame Кадр видеопотока
     */
    void ProcessFrame(const QVideoFrame &frame);
};

//
class VHWidgetCamera : public QWidget
{

    Q_OBJECT

public:
    explicit VHWidgetCamera(QWidget *parent = nullptr);

signals:

    // emitted in ProcessFrame
    void frameReady(const QImage &img);

private slots:

    // Event from button
    void OnClickCameraStart();

    // Event from button
    void OnClickCameraStop();

    // Event from VHCameraObject
    void onFrameReady(const QImage &img);

private:

    QWidget *parent;

    // camera object
    VHCameraObject camera;

    //
    QPushButton *btnCameraStart;
    QPushButton *btnCameraStop;

    // Available CAM List
    QComboBox *cmbCameraList;

    // Supported Resolutions List
    QComboBox *cmbResolutionsList;

    //
    WidgetImage *wdgImage;

    //
    void Init();

    //
    void InitObjects();

    //
    void InitLayout();

    //
    void StartCamera();

    //
    void StopCamera();

    // auto vlayleft = new QVBoxLayout();
    // vlayleft->addWidget(btnCameraStart);
};
