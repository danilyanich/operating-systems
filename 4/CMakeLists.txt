cmake_minimum_required(VERSION 3.15)
project(file-system)

Include(FetchContent)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.1)

FetchContent_MakeAvailable(Catch2)

target_link_libraries(tests Catch2::Catch2)

set(CMAKE_CXX_STANDARD 17)
add_executable(file-system main.cpp)