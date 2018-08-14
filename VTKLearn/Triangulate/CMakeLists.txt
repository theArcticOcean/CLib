
cmake_minimum_required(VERSION 2.8)
 
PROJECT(Triangulate)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(Triangulate MACOSX_BUNDLE Triangulate.cxx )
 
target_link_libraries(Triangulate ${VTK_LIBRARIES})
