cmake_minimum_required(VERSION 2.8)

project(ExportTest)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

set( OPENCTM_LIB_DIR /Users/weiyang/Downloads/OpenCTM-1.0.3/lib )
include_directories( ${OPENCTM_LIB_DIR} )
link_directories( ${OPENCTM_LIB_DIR} )

#message( "OPENCTM_H_FILES: "${OPENCTM_LIB_DIR} )
#message( "VTK_LIBRARIES: "${VTK_LIBRARIES} )

add_executable( ${PROJECT_NAME}  "main.cpp" "tool.h" )

# we need libopenctm.dylib
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} openctm )
