cmake_minimum_required(VERSION 3.16)
project(ArgentumServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(single_client client/single_client.cpp)
add_executable(single_server server/single_server.cpp)
add_executable(multiply_clients server/multiply_clients.cpp)