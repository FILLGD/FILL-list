
cmake_minimum_required(VERSION 3.21)

project(FILLList VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(FILLList SHARED
    main.cpp
)

setup_geode_mod(FILLList)
