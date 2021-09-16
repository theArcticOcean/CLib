cmake_minimum_required(VERSION 2.8)

project(basicIO)

# =========== turn on C++17 ===========
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if ( CMAKE_SYSTEM_NAME MATCHES "Windows" )
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++lastest")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std:c++lastest")
endif()
# =========== finish: turn on C++17 ===========

add_executable(${PROJECT_NAME} "main.cpp" "BasicIO.h" "BasicIO.cpp" "dirent_win.h" "log.hpp")
