project(createPlane)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
