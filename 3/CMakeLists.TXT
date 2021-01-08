cmake_minimum_required(VERSION 3.14)
project(TCPMessenger)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server ExecutorServer.cpp)
add_executable(Client ExecutorClient.cpp)

if(WIN32)
    target_link_libraries(Server wsock32 ws2_32)
endif()