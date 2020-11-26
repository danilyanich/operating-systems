//
// Created by illfate on 11/26/20.
//
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include <catch2/catch.hpp>
#include "../src/PhysicalMemory.h"

TEST_CASE("test physical memory CRD") {
    PhysicalMemory physicalMemory(std::string(100, ' '));
    physicalMemory.Add("123456789876");
    REQUIRE(physicalMemory.Read(0, 2) == "123456789876    ");

    physicalMemory.Delete(0, 2);
    REQUIRE(physicalMemory.Read(0, 2) == std::string(16, ' '));
}

TEST_CASE("test physical memory write") {
    PhysicalMemory physicalMemory(std::string(100, ' '));
    physicalMemory.Add("1234567898765432");
    REQUIRE(physicalMemory.Read(0, 2) == "1234567898765432");

    physicalMemory.Write(0, 2, "9876543210");
    REQUIRE(physicalMemory.Read(0, 2) == "9876543210      ");
}