cmake_minimum_required(VERSION 3.17)
project(os4)

set(CMAKE_CXX_STANDARD 20)

add_executable(os4 main.cpp FileSystem.cpp FileSystem.h File.cpp File.h Table.cpp Table.h Directory.cpp Directory.h)