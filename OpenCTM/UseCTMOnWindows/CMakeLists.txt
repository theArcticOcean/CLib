cmake_minimum_required(VERSION 2.8)
#if(POLICY CMP0020)
#  cmake_policy(SET CMP0020 NEW)
#endif()
#if(POLICY CMP0015)
#  cmake_policy(SET CMP0015 NEW)
#endif()

project(UseCTMOnWindows)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

#E:\project\masterBranch\MainApp\Src\thirdparty\OpenCTM_1_0_3
# ..\..\..\..\masterBranch\MainApp\Src\thirdparty\OpenCTM_1_0_3\lib\
# ../../../../masterBranch/MainApp/Src/thirdparty/OpenCTM_1_0_3/lib/
#set( OPENCTM_HOME_DIR ../../../../masterBranch/MainApp/Src/thirdparty/OpenCTM_1_0_3 )
set( OPENCTM_LIB_DIR ../../../../../Files/OpenCTM-1.0.3/lib )
#set( OPENCTM_INCLUDE_DIR ${OPENCTM_HOME_DIR}/include )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin )

include_directories( ${OPENCTM_LIB_DIR} )


#add_library(openctm SHARED IMPORTED)

#set_property(TARGET openctm PROPERTY IMPORTED_LOCATION_RELEASE ${OPENCTM_LIB_DIR}/openctm.lib)
#set_property(TARGET openctm PROPERTY IMPORTED_LOCATION_DEBUG   ${OPENCTM_LIB_DIR}/openctm.lib)

message( "======>    OPENCTM_LIB_DIR: ${OPENCTM_LIB_DIR}" )
message( "======>    CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}" )

link_directories(
    ${OPENCTM_LIB_DIR}
    ${CMAKE_BINARY_DIR}
    )

add_executable( ${PROJECT_NAME} "main.cpp" "tool.h" )

# we need libopenctm.dylib
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} openctm.lib )
