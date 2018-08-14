
cmake_minimum_required(VERSION 2.8)
 
PROJECT(ReadSTL)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(ReadSTL MACOSX_BUNDLE ReadSTL.cxx )
 
target_link_libraries(ReadSTL ${VTK_LIBRARIES})
