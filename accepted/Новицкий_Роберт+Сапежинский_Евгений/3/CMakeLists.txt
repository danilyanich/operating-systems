cmake_minimum_required(VERSION 3.17)
project(os3)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE
        SocketAddress.cpp
        SocketAddress.h
        TCPSocket.cpp
        TCPSocket.h)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(os3 main.cpp ${SOURCE})
add_executable(center center.cpp ${SOURCE})