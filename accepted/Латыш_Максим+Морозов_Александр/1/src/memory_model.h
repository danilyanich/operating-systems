#include "memory.h"

typedef struct physical_address
{
    MemAddr start;
    size_t segment_length;
    struct physical_address *next;
} segment_queue;

typedef struct
{
    size_t physical_address;
    size_t memory_size;
    segment_queue *first_segment;
} memory_model;
