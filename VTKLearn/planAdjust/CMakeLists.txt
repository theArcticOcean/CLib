cmake_minimum_required(VERSION 2.8)

project(planAdjust)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

set( VTKSRC_DIR /Users/weiyang/Downloads/VTK-8.1.1 )

file(GLOB CXX_FILES
    *.cxx
    *.cpp
    #${VTKSRC_DIR}/*/*/*.cxx
    )

file(GLOB HEADER_FILES
    *.hpp
    *.h
    )


add_executable(${PROJECT_NAME} ${CXX_FILES} ${HEADER_FILES} )

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
