cmake_minimum_required(VERSION 3.16)
project(FileToxin)

set(CMAKE_CXX_STANDARD 14)

add_executable(filesystem main.cpp
        memory/hard/PhysicalMemory.cpp
        memory/hard/PhysicalMemory.h
        memory/hard/IndexesTable.cpp
        memory/hard/IndexesTable.h memory/soft/File.cpp memory/soft/File.h memory/soft/System.cpp memory/soft/System.h)