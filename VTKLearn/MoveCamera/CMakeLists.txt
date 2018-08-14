
cmake_minimum_required(VERSION 2.8)
 
PROJECT(MoveCamera)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(MoveCamera MACOSX_BUNDLE MoveCamera.cxx )
 
target_link_libraries(MoveCamera ${VTK_LIBRARIES})
