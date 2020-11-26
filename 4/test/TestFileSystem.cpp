//
// Created by illfate on 11/27/20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include "../src/PhysicalMemory.h"
#include "../src/FileSystem.h"

TEST_CASE("test file CRUD") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateFile("file1", "body\n hi");
    auto file = fs.ReadFromFile("file1");
    REQUIRE(file->GetBody() == "body\n hi");

    fs.WriteToFile(file->GetName(), "new body");
    file = fs.ReadFromFile("file1");
    REQUIRE(file->GetBody() == "new body");

    fs.DeleteFile(file->GetName());
    try {
        file = fs.ReadFromFile("file1");
    } catch (const std::exception &ex) {
        REQUIRE(ex.what() == std::string("map::at"));
    }
}

TEST_CASE("test directories") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.ChangeDirectory("dir1");

    auto dir2 = fs.CreateDir("dir2");
    fs.CreateDir("dir3");

    fs.DeleteDir(dir2->GetName());
    try {
        dir2 = fs.GetDir("dir2");
    } catch (const std::exception &ex) {
        REQUIRE(ex.what() == std::string("map::at"));
    }
    auto dir3 = fs.GetDir("dir3");
    REQUIRE(dir3->GetName() == "dir3");

    fs.Dump();
}

TEST_CASE("test copying files") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.ChangeDirectory("dir1");

    auto dir2 = fs.CreateDir("dir2");
    fs.CreateDir("dir3");

    fs.DeleteDir(dir2->GetName());
    try {
        dir2 = fs.GetDir("dir2");
    } catch (const std::exception &ex) {
        REQUIRE(ex.what() == std::string("map::at"));
    }
    auto dir3 = fs.GetDir("dir3");
    REQUIRE(dir3->GetName() == "dir3");
}

TEST_CASE("test moving files") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.ChangeDirectory("dir1");

    auto dir2 = fs.CreateDir("dir2");
    fs.CreateDir("dir3");

    fs.DeleteDir(dir2->GetName());
    try {
        dir2 = fs.GetDir("dir2");
    } catch (const std::exception &ex) {
        REQUIRE(ex.what() == std::string("map::at"));
    }
    auto dir3 = fs.GetDir("dir3");
    REQUIRE(dir3->GetName() == "dir3");
}

TEST_CASE("test hierarchy") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.ChangeDirectory("dir1");

    auto dir2 = fs.CreateDir("dir2");
    fs.CreateDir("dir3");

    fs.DeleteDir(dir2->GetName());
    try {
        dir2 = fs.GetDir("dir2");
    } catch (const std::exception &ex) {
        REQUIRE(ex.what() == std::string("map::at"));
    }
    auto dir3 = fs.GetDir("dir3");
    REQUIRE(dir3->GetName() == "dir3");
}