cmake_minimum_required(VERSION 2.8.8)
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0025)
  cmake_policy(SET CMP0025 NEW) # CMake 3.0
endif()
if(POLICY CMP0043)
  cmake_policy(SET CMP0043 NEW) # CMake 3.0
endif()
if(POLICY CMP0053)
  cmake_policy(SET CMP0053 NEW) # CMake 3.1
endif()

# =========== turn on C++17 ===========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if ( CMAKE_SYSTEM_NAME MATCHES "Windows" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++lastest")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++lastest")
endif()
# =========== finish: turn on C++17 ===========

set(CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_STATIC_LIBRARY_SUFFIX})

project(projectB)
find_package(VTK NO_MODULE)

include(${VTK_USE_FILE})

#find_path(HearderPath Hearder_Path)
#set( projectBHDir  ${HearderPath}/projectB)
#message( "=========> ${projectBHDir}" )

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
 )

# Set your files and resources here
set( Srcs 
	Base.cpp )
set( Hdrs
	PreDefinition.hpp
	Base.h
        )

#add_definitions(
#	-DCLASSEXPORT
#)

set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

message("==== projectB ===> ${CMAKE_CURRENT_SOURCE_DIR}")

if (APPLE)
   add_library(${PROJECT_NAME} STATIC ${Srcs} ${Hdrs})
   target_link_libraries(projectB ${VTK_LIBRARIES} )
else () #WINDOWS
   add_library(${PROJECT_NAME} SHARED ${Srcs} ${Hdrs})
   target_link_libraries(projectB ${VTK_LIBRARIES} )
endif(APPLE)


if (WIN32)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif(WIN32)

#foreach( _header ${Hdrs} )
#	STRING(REGEX MATCH "(.*)[/\]" DIR ${_header})
#        file(INSTALL ${_header} DESTINATION ${projectBHDir}/${DIR})
#endforeach( _header )
