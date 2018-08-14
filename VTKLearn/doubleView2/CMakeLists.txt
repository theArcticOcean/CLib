project(doubleView2)

cmake_minimum_required(VERSION 2.8)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

file(GLOB UI_FILES *.ui)
file(GLOB QT_WRAP *.h)
file(GLOB CXX_FILES *.cxx *.cpp)

if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_ui(UISrcs ${UI_FILES} )
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  add_executable(doubleView2 MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${QT_WRAP} )
  qt5_use_modules(doubleView2 Core Gui)
  target_link_libraries(doubleView2 ${VTK_LIBRARIES})
else()
  QT4_WRAP_UI(UISrcs ${UI_FILES})
  QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
  add_executable(doubleView2 MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${MOCSrcs})
  target_link_libraries(doubleView2 ${VTK_LIBRARIES})
endif()
