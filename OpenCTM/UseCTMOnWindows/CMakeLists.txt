cmake_minimum_required(VERSION 2.8)

project(UseCTMOnWindows)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

# /e/Files/OpenCTM-1.0.3/lib
set( OPENCTM_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../../../../Files/OpenCTM-1.0.3/lib )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

include_directories( ${OPENCTM_LIB_DIR} )

message( "======>    OPENCTM_LIB_DIR: ${OPENCTM_LIB_DIR}" )
message( "======>    CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}" )
message( "======>    CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}" )

link_directories( ${OPENCTM_LIB_DIR} )

add_executable( ${PROJECT_NAME} "main.cpp" "tool.h" )

# we need openctm.lib
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} openctm.lib )
