
cmake_minimum_required(VERSION 2.8)
 
PROJECT(TransformActor)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(TransformActor MACOSX_BUNDLE TransformActor.cxx )
 
target_link_libraries(TransformActor ${VTK_LIBRARIES})
