cmake_minimum_required(VERSION 3.12)
project(FRPCPP LANGUAGES CXX)

#SET(CMAKE_C_COMPILER "/usr/local/bin/gcc")tt
#SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -g -std=c++20 -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -std=c++20 -pthread")
set (EXECUTABLE_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")

add_subdirectory(src)