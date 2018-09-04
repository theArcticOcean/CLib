cmake_minimum_required(VERSION 2.8)

PROJECT(FrustumPlane)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(${PROJECT_NAME} MACOSX_BUNDLE main.cpp MyStyle.h MyStyle.cpp  )

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})
