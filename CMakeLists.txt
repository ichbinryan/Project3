cmake_minimum_required(VERSION 3.6)
project(Project3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES graph.c graph.h simulator.c simulator.h)
add_executable(Project3 ${SOURCE_FILES})