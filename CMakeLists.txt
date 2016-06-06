# Copyright (C) 2016 David Capello

cmake_minimum_required(VERSION 2.8)

project(format CXX)

if(UNIX)
  add_definitions(-std=c++11)
endif()

include_directories(.)

# Examples
add_executable(hello_world hello_world.cpp)

# Tests
enable_testing()
add_subdirectory(tests)
