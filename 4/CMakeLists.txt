cmake_minimum_required(VERSION 3.15)
project(file-system)

Include(FetchContent)
set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG        v2.13.1)

FetchContent_GetProperties(Catch2) #mispelled name in original post
if(NOT catch2_POPULATED)   # name is lowercased
    FetchContent_Populate(Catch2)
    message(STATUS "Catch source dir: ${catch2_SOURCE_DIR}")
    message(STATUS "Catch binary dir: ${catch2_BINARY_DIR}")
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR}) # name is lowercased
endif()

add_executable(file-system src/main.cpp src/PhysicalMemory.h src/Block.h src/FileSystemEntity.h src/File.h src/Directory.h src/FileSystem.h)
add_executable(tests test/TetsPhysicalMemory.cpp)

enable_testing()
add_test(NAME tests COMMAND test_suite1)

target_link_libraries(tests Catch2::Catch2)

