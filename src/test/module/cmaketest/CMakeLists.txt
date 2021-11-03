cmake_minimum_required(VERSION 3.18)
project(cpp20-modules)

#SET(CMAKE_C_COMPILER "/usr/local/bin/gcc")
#SET(CMAKE_CXX_COMPILER "/usr/local/bin/g++")

# Add target to build iostream module
add_custom_target(std_modules ALL
    COMMAND ${CMAKE_COMMAND} -E echo "Building standard library modules"
    COMMAND g++ -fmodules-ts -std=c++20 -c -x c++-system-header iostream
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

function (prepare_for_module PROGRAM)
    target_compile_options(${PROGRAM} PUBLIC -fmodules-ts)
    set_property(TARGET ${PROGRAM} PROPERTY CXX_STANDARD 20)
    set_property(TARGET ${PROGRAM} PROPERTY CXX_EXTENSIONS OFF)
    add_dependencies(${PROGRAM} std_modules)
endfunction()

#set (TARGET prog)
#set (MODULE_TARGET prog-modules)

add_library(prog-modules OBJECT mymod.cpp)
prepare_for_module(prog-modules)

add_executable(prog main.cpp)
prepare_for_module(prog)

target_link_libraries(prog PRIVATE prog-modules)