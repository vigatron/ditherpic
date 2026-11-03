cmake_minimum_required(VERSION 3.16)

project(appdithervhpic VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Подключаем папку с субмодулем
add_subdirectory(src/extmods/vhlibfsys)

# Подключаем папку с субмодулем
add_subdirectory(src/extmods/vhlibcamera)


# Настройки для Qt6
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Multimedia)


include_directories(src)
include_directories(src/mods)
include_directories(src/mods/tables)
include_directories(src/mods/sys)
include_directories(src/mods/vhpal256)
include_directories(src/mods/vhpic256)
include_directories(src/mods/dithering)
include_directories(src/mods/wdg)
include_directories(src/mods/wdg/wdgimagepanel)
include_directories(src/mods/wdg/wdgsettings)
include_directories(src/mods/wdg/wdgapp)

# Явный список файлов вместо GLOB_RECURSE
add_executable(appdithervhpic
    src/appdithering.cpp
    src/mods/dithering/dithering.cpp
    src/mods/sys/sysfuncs.cpp
    src/mods/vhpal256/vhcolors256.cpp
    src/mods/wdg/wdgapp/wdgDitheringWnd.cpp
    src/mods/wdg/wdgformat.cpp
    src/mods/wdg/wdgimagepanel/wdgimagepanel.cpp
    src/mods/wdg/wdgsettings/wdgsettings.cpp
)

#
target_link_libraries(appdithervhpic PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Multimedia
    vhlib_fsys
    vhlib_camera
)
