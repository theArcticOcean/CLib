
cmake_minimum_required(VERSION 2.8)
 
PROJECT(MultipleViewports)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(MultipleViewports MACOSX_BUNDLE MultipleViewports.cxx )
 
target_link_libraries(MultipleViewports ${VTK_LIBRARIES})
