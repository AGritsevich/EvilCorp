cmake_minimum_required (VERSION 2.8.11)
set(APP_NAME EvilCorp)

project (${APP_NAME})

if(CMAKE_BUILD_TYPE MATCHES "")
	message(STATUS "BUILD_TYPE doesn't set, going set Debug")
    set(CMAKE_BUILD_TYPE Debug)
endif(CMAKE_BUILD_TYPE MATCHES "")
message(STATUS "BUILD_TYPE: " ${CMAKE_BUILD_TYPE})

add_compile_options(-std=c++11) # CMake 2.8.12 or newer

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory (src/)
