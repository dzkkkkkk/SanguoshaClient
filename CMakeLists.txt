cmake_minimum_required(VERSION 3.5)
project(SanguoshaClient LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 自动处理Qt的moc、uic、rcc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找所需的库
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
find_package(Protobuf REQUIRED)

# 包含目录
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/proto
    ${CMAKE_CURRENT_SOURCE_DIR}/network
    ${Protobuf_INCLUDE_DIRS}
)

# 添加可执行文件
add_executable(SanguoshaClient
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    network/networkmanager.cpp
    network/networkmanager.h
    proto/sanguosha.pb.cc
    proto/sanguosha.pb.h
)

# 链接库
target_link_libraries(SanguoshaClient
    Qt5::Widgets
    Qt5::Network
    ${Protobuf_LIBRARIES}
)