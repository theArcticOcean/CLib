project(sortCirclePts)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

add_executable(${PROJECT_NAME} "main.cpp" "ULineSegmentsToConnectedList.h" "ULineSegmentsToConnectedList.cpp" )

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
