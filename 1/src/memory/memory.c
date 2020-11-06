#include <stdlib.h>
#include <string.h>
#include <printf.h>

#include "memory.h"

char *_g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;



void m_malloc() {
    pagesTable[0].firstBit = _g_allocator_memory;
    pagesTable[0].isTaken = false;

    for (int i = 1; i < NUMBER_OF_PAGES; i++) {
        pagesTable[i].firstBit = pagesTable[i - 1].firstBit + 5;
        pagesTable[i].isTaken = false;
    }
}


void m_free(int ptr, m_err_code *error) {
    if (ptr <= NUMBER_OF_PAGES) {
        if (pagesTable[ptr - 1].isTaken) {
            pagesTable[ptr - 1].isTaken = false;
            memset(&pagesTable[ptr - 1].chunk, 0, sizeof(pagesTable[ptr - 1].chunk));
            *error = M_ERR_OK;
        } else *error = M_ERR_ALREADY_DEALLOCATED;
    } else *error = M_ERR_INVALID_CHUNK;
}


void m_read(int read_from_id, void *read_to_buffer, int size_to_read, m_err_code *error) {
    int counter = 0;
    if (read_from_id <= NUMBER_OF_PAGES) {
        for (int i = 0; i < 5; i++) {
            if (pagesTable[read_from_id].chunk.data[i] != '\0') {
                counter++;
            }
        }
        if (size_to_read <= counter) {
            memcpy(read_to_buffer, pagesTable[read_from_id - 1].chunk.data, size_to_read);
            *error = M_ERR_OK;
        } else *error = M_ERR_OUT_OF_BOUNDS;
    } else *error = M_ERR_INVALID_CHUNK;
}

void initializeChunk(void *write_from_buffer, int size_to_write, int number_of_page) {
    struct Chunk chunk;
    chunk.firstBitOfChunk = pagesTable[number_of_page].firstBit;
    chunk.size = size_to_write;
    strncpy(chunk.data, write_from_buffer, 5);
    pagesTable[number_of_page].chunk = chunk;
    pagesTable[number_of_page].isTaken = true;
}

void fillSeveralPages(void *write_from_buffer, char *firstBitOfFirstPage, int numberOfPages) {
    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        if (pagesTable[i].firstBit == firstBitOfFirstPage) {
            for (int j = 0; j < numberOfPages; j++) {
                if (sizeof(write_from_buffer) > 5) {
                    char temp[6];
                    strncpy(temp, write_from_buffer, 5);
                    initializeChunk(temp, 5, i + j);
                    write_from_buffer = write_from_buffer + 5;
                } else initializeChunk(write_from_buffer, sizeof(write_from_buffer), i + j);
            }
            break;
        }
    }
}

void m_write(void *write_from_buffer, int size_to_write, m_err_code *error) {
    int numberOfPages;
    if (size_to_write % 5 == 0) numberOfPages = size_to_write / 5; else numberOfPages = size_to_write / 5 + 1;
    int counter = numberOfPages;
    if (numberOfPages > 10) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return;
    }
    if (numberOfPages == 1) {
        for (int i = 0; i < NUMBER_OF_PAGES; i++) {
            if (!pagesTable[i].isTaken) {
                initializeChunk(write_from_buffer, size_to_write, i);
                break;
            }
        }
    } else {
        for (int i = 0; i < NUMBER_OF_PAGES; i++) {
            if (!pagesTable[i].isTaken) counter--; else counter = numberOfPages;
            if (counter == 0) {
                char *firstBitOfFirstPage = pagesTable[i - numberOfPages + 1].firstBit;
                fillSeveralPages(write_from_buffer, firstBitOfFirstPage, numberOfPages);
                break;
            }
        }

    }
}

void listAllBlocks() {
    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        printf("\n");
        printf(pagesTable[i].chunk.data);
    }
}

void m_init() {
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);

    _g_allocator_memory_size = NUMBER_OF_PAGES * SIZE_OF_PAGE;
    _g_allocator_memory = malloc(_g_allocator_memory_size);
}
