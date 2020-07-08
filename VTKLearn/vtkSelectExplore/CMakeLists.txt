cmake_minimum_required(VERSION 2.8)

project(vtkSelectExplore)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

add_executable(${PROJECT_NAME} "main.cpp"
    "UVtkSelectPolyData.h"
    "UVtkSelectPolyData.cpp"
    "ULog.h"
	"UMacroDefinition.h"
	"Tools.h"
    )

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )

if (WIN32)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
endif(WIN32)
