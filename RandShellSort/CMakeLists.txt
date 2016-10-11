cmake_minimum_required(VERSION 2.8.4)
project(RandShellSort)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")

set(SOURCE_FILES
    src/IO.h
    src/main.cpp
    src/RandShellSort.cpp
    src/RandShellSort.h
    )

add_executable(RandShellSort ${SOURCE_FILES})