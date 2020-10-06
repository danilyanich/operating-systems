//
// Created by Petri on 9/13/2020.
//

#ifndef UNTITLED_MEMORY_MODEL_H
#define UNTITLED_MEMORY_MODEL_H

#include "mmemory.h"

typedef struct physical_address {
    VA va;
    size_t segment_length;
    struct physical_address *next;
} segment_queue;

typedef struct {
    size_t physical_address;
    size_t memory_size;
    segment_queue *first_segment;
} memory_nodel;

#endif //UNTITLED_MEMORY_MODEL_H
