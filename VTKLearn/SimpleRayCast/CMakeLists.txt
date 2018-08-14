
cmake_minimum_required(VERSION 2.8)
 
PROJECT(SimpleRayCast)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(SimpleRayCast MACOSX_BUNDLE SimpleRayCast.cxx )
 
target_link_libraries(SimpleRayCast ${VTK_LIBRARIES})
