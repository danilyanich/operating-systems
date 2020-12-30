cmake_minimum_required(VERSION 3.17)
project(os2)

set(CMAKE_CXX_STANDARD 20)

SET(CMAKE_CXX_FLAGS -pthread)

add_executable(os2 main.cpp Barber.cpp Barber.h User.cpp User.h Manager.cpp Manager.h)