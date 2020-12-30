# cmake_minimum_required(VERSION <specify CMake version here>)
project(1)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/memory)

add_executable(1
        src/memory/memory.c
        src/memory/memory.h
        src/main.c
        .gitignore
        CMakeLists.txt
        README.md)
