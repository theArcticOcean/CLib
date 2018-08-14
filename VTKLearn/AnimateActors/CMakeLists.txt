
cmake_minimum_required(VERSION 2.8)
 
PROJECT(AnimateActors)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(AnimateActors MACOSX_BUNDLE AnimateActors.cxx )
 
target_link_libraries(AnimateActors ${VTK_LIBRARIES})
