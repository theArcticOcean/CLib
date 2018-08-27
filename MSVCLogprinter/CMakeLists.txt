project(MSVCLogprinter)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

find_package(Qt5 COMPONENTS Core REQUIRED QUIET )

add_executable(${PROJECT_NAME} ${SRC_LIST})
qt5_use_modules( ${PROJECT_NAME} Core Widgets )
