cmake_minimum_required(VERSION 3.31)
project(programForArch)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Widgets
        REQUIRED)

add_executable(programForArch main.cpp)
target_link_libraries(programForArch
        Qt::Core
        Qt::Widgets

)

