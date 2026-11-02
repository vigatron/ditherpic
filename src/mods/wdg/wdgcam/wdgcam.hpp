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

//
class VHWidgetCamera : public QWidget
{

    Q_OBJECT

public:
    explicit VHWidgetCamera(QWidget *parent = nullptr);

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

    // emitted in ProcessFrame
    void frameReady(const QImage &img);

private slots:

    // Event from button
    void OnClickCameraStart();

    // Event from button
    void OnClickCameraStop();

private:

    QWidget *parent;

    // --- Camera part (was VHCameraObject) ---

    QCamera *camera;
    QList<QCameraDevice> camerasList;
    QMediaCaptureSession session;
    QVideoSink *sink;

    bool firstsnap = false;

    // --- Widget part ---

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

    // auto vlayleft = new QVBoxLayout();
    // vlayleft->addWidget(btnCameraStart);
};
