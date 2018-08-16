cmake_minimum_required(VERSION 2.8)

project(exploreCamera)
add_executable(${PROJECT_NAME} MACOSX_BUNDLE "main.cpp")

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
