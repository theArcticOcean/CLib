project(ThresholdCells)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

add_executable(${PROJECT_NAME} "main.cpp" "tool.h")

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
