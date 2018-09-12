project(MSVCLogprinter)
cmake_minimum_required(VERSION 2.8)

find_package(Qt5 COMPONENTS Core REQUIRED QUIET )

add_executable(${PROJECT_NAME} main.cpp logprinter.cpp logprinter.h )
qt5_use_modules( ${PROJECT_NAME} Core Widgets )
