
cmake_minimum_required(VERSION 2.8)
 
PROJECT(ProjectedTexture)
 
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})
 
add_executable(ProjectedTexture MACOSX_BUNDLE ProjectedTexture.cxx )
 
target_link_libraries(ProjectedTexture ${VTK_LIBRARIES})
