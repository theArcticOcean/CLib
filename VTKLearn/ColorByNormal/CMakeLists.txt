
cmake_minimum_required(VERSION 2.8)
 
PROJECT(ColorByNormal)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(ColorByNormal MACOSX_BUNDLE ColorByNormal.cxx )
 
target_link_libraries(ColorByNormal ${VTK_LIBRARIES})
