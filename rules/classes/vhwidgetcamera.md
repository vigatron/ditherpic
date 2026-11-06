# VHWidgetCamera

`VHWidgetCamera` - QWidget который содержит в себе элементы управления и
необходимые объекты и классы для захвата картинки с видеопотока от `QCamera`


## Состав

- кнопки управления btnCameraStart / btnCameraPause / btnCameraStop
- окно вывода изображения камеры QImage based
- QComboBox * cmbCameraList для выбора камеры из списка доступных устройств
- QComboBox * cmbFormatList для выбора формата видеопотока


### cmbCameraList

Позволяет выбирать источник картинки ( камеру ) по id либо по имени.

### cmbFormatList

Supported video formats list for selected camera

### Кнопки виджета

`QPushButton * btnCameraStart` - Инициализирует `QCamera`

`QPushButton * btnCameraPause` - ( не реализовано, требует реализации ) Нажатие ставит видеопоток на паузу, повторное нажатие - снимает с паузы.

`QPushButton * btnCameraStop`  - Полностью деинициализирует `QCamera`


### Трансляция стрима с камеры

Обновляется `VHWidgetCamImage` (based on QImage) если камера инициализирована.
