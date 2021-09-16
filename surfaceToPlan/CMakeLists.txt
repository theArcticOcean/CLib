cmake_minimum_required(VERSION 2.8)

project(vtkLearn)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

add_executable(${PROJECT_NAME} "main.cpp"
    "UPolyDataSurfaceToPlane.h"
    "UPolyDataSurfaceToPlane.cpp"
    "tool.h")

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
