cmake_minimum_required(VERSION 3.12)
project(draco_for_compress)

# =========== turn on C++17 ===========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# Find Draco library
find_package(draco REQUIRED)

# Include directories
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${draco_INCLUDE_DIRS}
)

# Create executable
add_executable(${PROJECT_NAME} "main_simpleIO_compare.cpp")

# Link libraries
target_link_libraries(${PROJECT_NAME} 
  ${VTK_LIBRARIES}
  draco::draco
)

# Set target properties for Draco
if(TARGET draco::draco)
  set_target_properties(draco::draco PROPERTIES
    IMPORTED_IMPLIB_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../draco/build/Release/draco.lib"
    #IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/../draco/build/Release/draco.dll"
  )
endif()