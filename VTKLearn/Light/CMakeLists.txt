
cmake_minimum_required(VERSION 2.8)
 
PROJECT(Light)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(Light MACOSX_BUNDLE Light.cxx )
 
target_link_libraries(Light ${VTK_LIBRARIES})
