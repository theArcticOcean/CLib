cmake_minimum_required(VERSION 2.8)

project(cmakeAddSubdirectory)

if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

add_subdirectory(projectA)
add_subdirectory(projectB)
