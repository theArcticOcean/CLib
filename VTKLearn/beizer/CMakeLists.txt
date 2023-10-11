cmake_minimum_required(VERSION 2.8)

project(vtk802)

find_package( VTK REQUIRED )

add_executable(${PROJECT_NAME}
    "main.cpp"
    "point.hpp"
    "CustomIteractorStyle.h"
    "CustomIteractorStyle.cpp"
    "ULog.h"
    "UPaintBrush.h"
    "UPaintBrush.cpp")


#find_package( VTK COMPONENTS vtkCommonCore vtkRenderingCore vtkInteractionStyle )

if (VTK_VERSION VERSION_LESS "8.90.0")
  # old system
  include(${VTK_USE_FILE})
else ()
  # vtk_module_autoinit is needed
  vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
    )
endif ()

target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )
