cmake_minimum_required(VERSION 3.14)
project(2)

set(CMAKE_CXX_STANDARD 14)

add_executable(2 main.cpp)

if(WIN32)
    target_link_libraries(2 wsock32 ws2_32)
endif()