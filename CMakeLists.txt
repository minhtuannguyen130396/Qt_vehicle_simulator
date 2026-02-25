cmake_minimum_required(VERSION 3.16)
project(QtDeviceApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick Qml)

add_subdirectory(lib/messageQueue)

add_executable(QtDeviceApp
    main.cpp

    controller/Controller.cpp
    controller/Controller.h

    ui/UiBackend.cpp
    ui/UiBackend.h

    # Worker layers (device -> middleware -> board simulator)
    devices/Device.cpp
    devices/Device.h

    middleware/HardwareService.cpp
    middleware/HardwareService.h
    middleware/HwTypes.cpp
    middleware/HwTypes.h

    board/SimBatteryDriver.cpp
    board/SimBatteryDriver.h
    board/SimMotorDriver.cpp
    board/SimMotorDriver.h
    board/IBatteryDriver.h
    board/IMotorDriver.h

    qml.qrc
)

target_include_directories(QtDeviceApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(QtDeviceApp
    messageQueue
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::Qml
)
