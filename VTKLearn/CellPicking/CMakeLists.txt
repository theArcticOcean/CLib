
cmake_minimum_required(VERSION 2.8)
 
PROJECT(CellPicking)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(CellPicking MACOSX_BUNDLE CellPicking.cxx )
 
target_link_libraries(CellPicking ${VTK_LIBRARIES})
