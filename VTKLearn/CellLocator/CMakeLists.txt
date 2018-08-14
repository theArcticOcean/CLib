
cmake_minimum_required(VERSION 2.8)
 
PROJECT(CellLocator)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(CellLocator MACOSX_BUNDLE CellLocator.cxx )
 
target_link_libraries(CellLocator ${VTK_LIBRARIES})
