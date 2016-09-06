cmake_minimum_required(VERSION 3.3)
project(TemplateMagic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

file(GLOB SOURCE_FILES
    **/*.cpp
    **/*.h)

add_executable(cpp-builder-template ${SOURCE_FILES})