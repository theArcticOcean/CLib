project(doubleView2)

cmake_minimum_required(VERSION 2.8)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

set( QTSRC_DIR /Users/weiyang/Qt5.9.2 )
set( VTKSRC_DIR /Users/weiyang/Downloads/VTK-8.1.1 )

file(GLOB UI_FILES *.ui)

file(GLOB QT_WRAP
    *.h
    #${QTSRC_DIR}/5.9.2/clang_64/lib/QtDesigner.framework/Versions/5/Headers/*
    #${VTKSRC_DIR}/*/*/*.h
    )

file(GLOB CXX_FILES
    *.cxx
    *.cpp
    #${VTKSRC_DIR}/*/*/*.cxx
    )

if(${VTK_VERSION} VERSION_GREATER "6" AND VTK_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_ui(UISrcs ${UI_FILES} )
  # CMAKE_AUTOMOC in ON so the MOC headers will be automatically wrapped.
  set( CMAKE_AUTOMOC ON )
  add_executable(doubleView2 MACOSX_BUNDLE  ${QT_WRAP} ${CXX_FILES} ${UISrcs} )
  qt5_use_modules(doubleView2 Core Gui)
  target_link_libraries(doubleView2 ${VTK_LIBRARIES})
else()
  QT4_WRAP_UI(UISrcs ${UI_FILES})
  QT4_WRAP_CPP(MOCSrcs ${QT_WRAP})
  add_executable(doubleView2 MACOSX_BUNDLE ${CXX_FILES} ${UISrcs} ${MOCSrcs})
  target_link_libraries(doubleView2 ${VTK_LIBRARIES})
endif()
