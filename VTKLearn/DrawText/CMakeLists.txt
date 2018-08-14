
cmake_minimum_required(VERSION 2.8)
 
PROJECT(DrawText)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(DrawText MACOSX_BUNDLE DrawText.cxx )
 
target_link_libraries(DrawText ${VTK_LIBRARIES})
