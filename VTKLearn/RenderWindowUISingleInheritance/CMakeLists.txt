
cmake_minimum_required(VERSION 2.8)
 
if(POLICY CMP0020)
  cmake_policy(SET CMP0020 NEW)
endif()

PROJECT(RenderWindowUISingleInheritance)

find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#SET(QT_USE_QTWIDGETS TRUE)
SET(QT_USE_QTMAIN TRUE)
## Find includes in corresponding build directories
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
# 
## Instruct CMake to run moc automatically when needed
#set(CMAKE_AUTOMOC ON)
# 
#set(EXECUTABLE_OUTPUT_PATH  "${PROJECT_SOURCE_DIR}/bin")
# 
# Find the QtWidgets library
find_package(Qt5Widgets)


if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  # Instruct CMake to run moc automatically when needed.
  set(CMAKE_AUTOMOC ON)
  find_package(Qt5Widgets REQUIRED QUIET)
else()
  find_package(Qt4 REQUIRED)
  include(${QT_USE_FILE})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx)

if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_ui(UISrcs ${UI_FILES} )
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(RenderWindowUISingleInheritance MACOSX_BUNDLE
    ${CXX_FILES} ${UISrcs} ${QT_WRAP})
  qt5_use_modules(RenderWindowUISingleInheritance Core Gui)
  target_link_libraries(RenderWindowUISingleInheritance ${VTK_LIBRARIES})
else()
  QT4_WRAP_UI(UISrcs ${UI_FILES})
  QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
  add_executable(RenderWindowUISingleInheritance MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${MOCSrcs})
  target_link_libraries(RenderWindowUISingleInheritance ${VTK_LIBRARIES})
endif()
