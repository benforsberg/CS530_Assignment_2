cmake_minimum_required(VERSION 3.15)
project(dxe)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(dxe
        main.cpp
        opcode.cpp
        opcode.h)
