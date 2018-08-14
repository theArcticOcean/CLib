project(doubleView)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )
target_link_libraries( doubleView ${VTK_LIBRARIES} )
