# Copyright (C) 2016 David Capello

cmake_minimum_required(VERSION 3.5)

project(format CXX)

add_definitions(-std=c++14)
include_directories(.)

# Examples
add_executable(hello_world hello_world.cpp)

# Tests
enable_testing()
add_subdirectory(tests)
