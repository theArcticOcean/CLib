cmake_minimum_required(VERSION 2.8)

project(glyph3D)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

add_executable(${PROJECT_NAME} "main.cpp" "point.hpp"
    "multiClipPolyData.h" "multiClipPolyData.cpp"
    "PolydataMethods.h" "PolydataMethods.cpp"
    )

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
