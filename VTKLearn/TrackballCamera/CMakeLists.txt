
cmake_minimum_required(VERSION 2.8)
 
PROJECT(TrackballCamera)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
message( "VTK_USE_FILE is ${VTK_USE_FILE}" )
 
add_executable(TrackballCamera MACOSX_BUNDLE TrackballCamera.cxx )
 
target_link_libraries(TrackballCamera ${VTK_LIBRARIES})
