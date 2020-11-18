#pragma once

#include "stdlib.h"

#define SIZE_OF_VIRTUAL_ADDRESS 16
#define SEGMENT_DELTA 1
#define ADDRESS_DELTA 1
#define EQUALS_CONDITION 0
#define INITIAL_OFFSET 0
#define INITIAL_SIZE 1
#define INITIAL_ADDRESS 0
#define DEFAULT_VALUE_OF_MEMORY 0
#define DEFAULT_VALUE_OF_ADDRESS "ffffffffffffffff"

typedef struct SegmentsTable {
    size_t totalMemory;
    size_t freeMemory;
    int size;
    PhysicalAddress initialAddress;
    Segment *firstSegment;
    int addressIncrement;
} SegmentsTable;

void setSegmentAddress(SegmentsTable *table, Segment *segment) {
    VA virtualAddress = malloc(SIZE_OF_VIRTUAL_ADDRESS);
    sprintf(virtualAddress, "%d", table->addressIncrement);
    segment->address = virtualAddress;
    table->addressIncrement += ADDRESS_DELTA;
}

Segment *createSegment(SegmentsTable *table, size_t segmentSize,
                       PhysicalAddress physicalAddress, bool isFree, int offset) {
    Segment *segment = malloc(sizeof(Segment));
    setSegmentAddress(table, segment);
    segment->size = segmentSize;
    segment->isFree = isFree;
    segment->offset = offset;
    segment->physicalAddress = physicalAddress;
    return segment;
}

SegmentsTable *initSegmentsTable(int size, int segmentSize) {
    SegmentsTable *table = malloc(sizeof(SegmentsTable));
    const size_t totalMemory = size * segmentSize;
    table->totalMemory = totalMemory;
    table->freeMemory = totalMemory;
    table->size = INITIAL_SIZE;
    table->addressIncrement = INITIAL_ADDRESS;
    table->initialAddress = malloc(totalMemory);
    Segment *initialSegment = createSegment(table, totalMemory, table->initialAddress, true, INITIAL_OFFSET);
    initialSegment->nextSegment = NULL;
    table->firstSegment = initialSegment;
    return table;
}

Segment *findFreeSegment(SegmentsTable *table, size_t segmentSize) {
    Segment *segment = table->firstSegment;
    for (int indexOfSegment = 0; indexOfSegment < table->size; indexOfSegment++) {
        if (segment == NULL) {
            break;
        }
        if (segment->isFree && segment->size >= segmentSize) {
            return segment;
        } else {
            segment = segment->nextSegment;
        }
    }
    return segment;
}

void fillSegment(SegmentsTable *table, Segment *segment, size_t segmentSize) {
    size_t sizeOfFreeSegment = segment->size - segmentSize;
    if (sizeOfFreeSegment != DEFAULT_VALUE_OF_MEMORY) {
        int offsetOfFreeSegment = segment->offset + segmentSize;
        PhysicalAddress addressOfFreeSegment = segment->physicalAddress + offsetOfFreeSegment;
        Segment *freeSegment = createSegment(table, sizeOfFreeSegment, addressOfFreeSegment, true, offsetOfFreeSegment);
        freeSegment->nextSegment = segment->nextSegment;
        segment->nextSegment = freeSegment;
    }
    setSegmentAddress(table, segment);
    segment->isFree = false;
    segment->size = segmentSize;
}

int allocateMemoryForSegment(SegmentsTable *table, VA *segmentAddress, size_t segmentSize) {
    if (table->freeMemory < segmentSize) {
        return OUT_OF_MEMORY;
    }
    Segment *segment = findFreeSegment(table, segmentSize);
    if (segment != NULL) {
        fillSegment(table, segment, segmentSize);
        *segmentAddress = segment->address;
        table->size += SEGMENT_DELTA;
        table->freeMemory -= segmentSize;
        return SUCCESSFUL_CODE;
    } else {
        return OUT_OF_MEMORY;
    }
}

Segment *findSegment(SegmentsTable *table, const VA addressOfSegment) {
    if (table->firstSegment == NULL || addressOfSegment == DEFAULT_VALUE_OF_ADDRESS) {
        return NULL;
    }
    Segment *segment = table->firstSegment;
    for (int indexOfSegment = 0; indexOfSegment < table->size; indexOfSegment++) {
        if (segment == NULL) {
            break;
        }
        if (strncmp(segment->address, addressOfSegment, segment->size) == EQUALS_CONDITION) {
            return segment;
        } else {
            segment = segment->nextSegment;
        }
    }
    return NULL;
}

int freeSegment(SegmentsTable *table, VA addressOfSegment) {
    Segment *segment = findSegment(table, addressOfSegment);
    if (segment != NULL) {
        table->freeMemory += segment->size;
        segment->isFree = true;
        memset(segment->physicalAddress, DEFAULT_VALUE_OF_MEMORY, segment->size);
        segment->address = DEFAULT_VALUE_OF_ADDRESS;
        return SUCCESSFUL_CODE;
    } else {
        return UNKNOWN_ERROR;
    }
}
