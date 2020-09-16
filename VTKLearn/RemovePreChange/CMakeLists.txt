cmake_minimum_required(VERSION 2.8)

project(RemovePreChange)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

add_executable(${PROJECT_NAME} "main.cpp" )

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
