#pragma once

#include "string.h"
#include "stdio.h"

#define EQUALS_CONDITION 0

void setGreenColor() {
    printf("\033[0;32m");
}

void setRedColor() {
    printf("\033[0;31m");
}

void setDefaultColor() {
    printf("\033[0m");
}

void assertEquals(const char *testName, int actualValue, int expectedValue) {
    if (actualValue == expectedValue) {
        setGreenColor();
        printf("Test %s is passed\n", testName);
    } else {
        setRedColor();
        printf("Test %s is failed, expected %d, but actual is %d \n", testName, expectedValue, actualValue);
    }
    setDefaultColor();
}

void assertArraysEquals(const char* testName, const char* actualValue, const char* expectedValue, int size) {
    if (strncmp(actualValue, expectedValue, size) == EQUALS_CONDITION) {
        setGreenColor();
        printf("Test %s is passed\n", testName);
    } else {
        setRedColor();
        printf("Test %s is failed, expected %s, but actual is %s \n", testName, expectedValue, actualValue);
    }
    setDefaultColor();
}