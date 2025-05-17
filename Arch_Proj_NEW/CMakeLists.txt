cmake_minimum_required(VERSION 3.30)
project(Arch_Proj_NEW)

set(CMAKE_CXX_STANDARD 20)

add_executable(Arch_Proj_NEW main.cpp
        PageEntry.h
        OperatingSystem.cpp
        MMU.cpp
        VMAlgorithms.cpp
        VMAlgorithms.h
)
