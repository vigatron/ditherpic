cmake_minimum_required(VERSION 3.28)

project(appdithervhpic VERSION 1.0 LANGUAGES CXX)

# Явно задаем компиляторы до project(), чтобы CMake подхватил их как дефолтные
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройки для Qt6
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

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



file( GLOB_RECURSE source_list_mods
    src/appdithering.cpp
    src/mods/tables
    src/mods/sys/*
    src/mods/vhpal256/*
    src/mods/vhpic256/*
    src/mods/dithering/*

    src/mods/wdg/*
    src/mods/wdg/wdgimagepanel/*
    src/mods/wdg/wdgapp/*
)

add_executable(appdithervhpic ${source_list_mods} )

target_link_libraries(appdithervhpic PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)
