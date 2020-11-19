#pragma once

#include <windef.h>
#include <afxres.h>

#include "../src/mmemory.c"

#define SEGMENTS_AMOUNT 65536
#define SEGMENT_SIZE 8096

void testCustomMallocAndFree() {
    _init(SEGMENTS_AMOUNT, SEGMENT_SIZE);
    DWORD start = GetTickCount();
    int i;
    for (i = 0; i < SEGMENTS_AMOUNT; i++) {
        VA address;
        _malloc(&address, SEGMENT_SIZE);
        _free(address);
    }
    DWORD end = GetTickCount();
    DWORD delta = end - start;
    printf("Time of custom malloc: %lu \n", delta);
}

void testSystemMallocAndFree() {
    DWORD start = GetTickCount();
    int i;
    for (i = 0; i < SEGMENTS_AMOUNT; i++) {
        void *address = malloc(SEGMENT_SIZE);
        free(address);
    }
    DWORD end = GetTickCount();
    DWORD delta = end - start;
    printf("Time of system malloc: %lu \n", delta);
}