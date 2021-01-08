cmake_minimum_required(VERSION 3.17)
project(file_system)

set(CMAKE_CXX_STANDARD 17)

add_executable(file_system main.cpp file_system.cpp file_system.h)
set(CXX_FILESYSTEM_LIBRARY "-lstdc++fs")
target_link_libraries(file_system ${CXX_FILESYSTEM_LIBRARY})