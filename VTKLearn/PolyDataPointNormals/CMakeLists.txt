
cmake_minimum_required(VERSION 2.8)
 
PROJECT(PolyDataPointNormals)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(PolyDataPointNormals MACOSX_BUNDLE PolyDataPointNormals.cxx )
 
target_link_libraries(PolyDataPointNormals ${VTK_LIBRARIES})
