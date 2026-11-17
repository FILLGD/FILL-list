
cmake_minimum_required(VERSION 3.21)

project(FILLList VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "GEODE_SDK bulunamadi!")
endif()

add_subdirectory(
    $ENV{GEODE_SDK}
    ${CMAKE_CURRENT_BINARY_DIR}/geode
)

file(GLOB_RECURSE SOURCES
    src/*.cpp
)

if (NOT SOURCES)
    set(SOURCES main.cpp)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

setup_geode_mod(${PROJECT_NAME})
