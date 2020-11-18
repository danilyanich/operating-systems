#pragma once

#include <stdbool.h>

typedef void *PhysicalAddress;

typedef struct Segment {
    VA address;
    size_t size;
    int offset;
    bool isFree;
    PhysicalAddress physicalAddress;
    struct Segment *nextSegment;
} Segment;
