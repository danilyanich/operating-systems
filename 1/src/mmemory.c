#pragma once
#include "string.h"

#include "mmemory.h"
#include "segment.c"
#include "segments_table.c"

#define MINIMAL_SEGMENTS_AMOUNT 1
#define MINIMAL_SEGMENT_SIZE 1
#define MINIMAL_BUFFER_SIZE 1

SegmentsTable *segmentsTable;

int _malloc(VA *address, size_t segmentSize) {
    if (address == NULL) {
        return WRONG_PARAMETERS;
    }
    if (segmentsTable == NULL) {
        return UNKNOWN_ERROR;
    }
    if (segmentSize < MINIMAL_SEGMENT_SIZE) {
        return WRONG_PARAMETERS;
    } else {
        return allocateMemoryForSegment(segmentsTable, address, segmentSize);
    }
}

int _free(VA address) {
    if (address == NULL) {
        return WRONG_PARAMETERS;
    }
    if (segmentsTable == NULL) {
        return UNKNOWN_ERROR;
    }
    return freeSegment(segmentsTable, address);
}

int validate(Segment **segment, VA address, void *addressOfBuffer, size_t bufferSize) {
    if (address == NULL) {
        return WRONG_PARAMETERS;
    }
    if (segmentsTable == NULL) {
        return UNKNOWN_ERROR;
    }
    if (addressOfBuffer == NULL) {
        return WRONG_PARAMETERS;
    }
    if (bufferSize < MINIMAL_BUFFER_SIZE) {
        return WRONG_PARAMETERS;
    }
    *segment = findSegment(segmentsTable, address);
    if (*segment == NULL) {
        return UNKNOWN_ERROR;
    }
    if ((*segment)->size < bufferSize) {
        return OUT_OF_MEMORY;
    }
    return SUCCESSFUL_CODE;
}

int _read(VA address, void *addressOfBuffer, size_t bufferSize) {
    Segment *segment;
    const int statusCode = validate(&segment, address, addressOfBuffer, bufferSize);
    if (statusCode == SUCCESSFUL_CODE) {
        memcpy(addressOfBuffer, segment->physicalAddress, bufferSize);
    }
    return statusCode;
}

int _write(VA address, void *addressOfBuffer, size_t bufferSize) {
    Segment *segment;
    const int statusCode = validate(&segment, address, addressOfBuffer, bufferSize);
    if (statusCode == SUCCESSFUL_CODE) {
        memcpy(segment->physicalAddress, addressOfBuffer, bufferSize);
    }
    return statusCode;
}

int _init(int segmentsAmount, int segmentSize) {
    if (segmentsTable != NULL) {
        return UNKNOWN_ERROR;
    }
    if (segmentsAmount < MINIMAL_SEGMENTS_AMOUNT || segmentSize < MINIMAL_SEGMENT_SIZE) {
        return WRONG_PARAMETERS;
    } else {
        segmentsTable = initSegmentsTable(segmentsAmount, segmentSize);
        return SUCCESSFUL_CODE;
    }
}