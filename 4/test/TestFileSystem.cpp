//
// Created by illfate on 11/27/20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include <sstream>
#include "../src/file-system/PhysicalMemory.h"
#include "../src/file-system/FileSystem.h"

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
        REQUIRE(ex.what() == std::string("not found"));
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
        REQUIRE(ex.what() == std::string("not found"));
    }
    auto dir3 = fs.GetDir("dir3");
    REQUIRE(dir3->GetName() == "dir3");
}

TEST_CASE("test copying files") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.CreateFile("file1", "body\n hi");
    auto file = fs.ReadFromFile("file1");
    REQUIRE(file->GetBody() == "body\n hi");

    fs.CopyFileToDir(file->GetName(), "dir1");
    fs.ChangeDirectory("dir1");
    file = fs.ReadFromFile("file1");
    REQUIRE(file->GetBody() == "body\n hi");
}

TEST_CASE("test moving files") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.CreateFile("file1", "body\n hi");
    auto file = fs.ReadFromFile("file1");
    REQUIRE(file->GetBody() == "body\n hi");

    fs.MoveFileToDir(file->GetName(), "dir1");

    fs.ChangeDirectory("dir1");
    file = fs.ReadFromFile("file1");
    REQUIRE(file->GetBody() == "body\n hi");
}

TEST_CASE("test hierarchy") {
    PhysicalMemory mem(std::string(100, ' '));
    FileSystem fs(mem);
    fs.CreateDir("dir1");
    fs.CreateFile("file1", "body1");
    fs.CreateFile("file2", "body2");
    fs.ChangeDirectory("dir1");
    fs.CreateFile("file3", "body3");
    fs.CreateDir("dir2");
    fs.CreateDir("dir3");
    fs.ChangeDirectory("dir3");
    fs.CreateFile("file4", "body4");

    std::stringstream ss;
    fs.Dump(ss);
    REQUIRE(ss.str() =="/\n"
                       "\tfile2\n"
                       "\tfile1\n"
                       "\tdir1\n"
                       "\t\tdir3\n"
                       "\t\t\tfile4\n"
                       "\t\tdir2\n"
                       "\t\tfile3\n");
}