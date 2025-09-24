cmake_minimum_required(VERSION 2.8)

project(setPrecisionForCTM)

# =========== turn on C++17 ===========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package( VTK REQUIRED )
include( ${VTK_USE_FILE} )

set( OpenCTMDir ${CMAKE_CURRENT_SOURCE_DIR}/OpenCTMLib )

set(openctm
  ${OpenCTMDir}/openctm.c
  ${OpenCTMDir}/compressMG1.c
  ${OpenCTMDir}/compressMG2.c
  ${OpenCTMDir}/compressRAW.c
  ${OpenCTMDir}/stream.c

  ${OpenCTMDir}/liblzma/Alloc.c
  ${OpenCTMDir}/liblzma/LzFind.c
  ${OpenCTMDir}/liblzma/LzmaDec.c
  ${OpenCTMDir}/liblzma/LzmaEnc.c
  ${OpenCTMDir}/liblzma/LzmaLib.c

  ${OpenCTMDir}/liblzma/LzmaLib.h
  ${OpenCTMDir}/openctm.h
  ${OpenCTMDir}/internal.h
  ${OpenCTMDir}/openctmpp.h

  ${OpenCTMDir}/liblzma/Alloc.h
  ${OpenCTMDir}/liblzma/LzFind.h
  ${OpenCTMDir}/liblzma/LzHash.h
  ${OpenCTMDir}/liblzma/LzmaDec.h
  ${OpenCTMDir}/liblzma/LzmaEnc.h
  ${OpenCTMDir}/liblzma/LzmaLib.h
  ${OpenCTMDir}/liblzma/Types.h
  ${OpenCTMDir}/liblzma/NameMangle.h
)

source_group(openctm FILES ${openctm})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${OpenCTMDir}
  ${OpenCTMDir}/liblzma
)

add_definitions(
   -DOPENCTM_BUILD
   -DLZMA_PREFIX_CTM
 )

message( "======>    OPENCTM_LIB_DIR: ${OPENCTM_LIB_DIR}" )
message( "======>    CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}" )
message( "======>    CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}" )

# link_directories( ${OPENCTM_LIB_DIR} )

add_executable( ${PROJECT_NAME} "main_read_compare.cpp" "PointStruct.hpp" "UCtmIO.h" "UCtmIO.cpp" "ULog.h" "UCommonIO.h" "UCommonIO.cpp" "dirent_win.h" "UMacroDefinition.h"  ${openctm}  )

# we need openctm.lib
target_link_libraries( ${PROJECT_NAME} ${VTK_LIBRARIES} )