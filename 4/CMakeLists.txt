cmake_minimum_required(VERSION 3.17)
project(4)

set(CMAKE_CXX_STANDARD 14)

add_executable(4 src/main.cpp src/CustomFile.cpp src/Cluster.cpp src/InterfaceCluster.cpp src/InterfaceFileSystem.cpp src/ReadWriteService.cpp src/DumpService.cpp src/Action.cpp src/ActionResult.cpp src/FileSystemService.cpp)