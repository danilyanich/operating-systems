#include "memory.h"
#include <stdlib.h>
#include <string>


char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;

void m_malloc(m_err_code* error) {
    pages_table[0].first_bit = _g_allocator_memory;
    pages_table[0].is_taken = false;
    for (int i = 1; i < NUMBER_OF_PAGES; i++) {
        pages_table[i].first_bit = pages_table[i - 1].first_bit + SIZE_OF_PAGE;
        pages_table[i].is_taken = false;
    }
    *error = M_ERR_OK;
}


void m_free(int free_page_id, m_err_code* error) {
    if (free_page_id <= NUMBER_OF_PAGES) {
        if (pages_table[free_page_id - 1].is_taken) {
            pages_table[free_page_id - 1].is_taken = false;
            memset(&pages_table[free_page_id - 1].chunk, 0, sizeof(pages_table[free_page_id - 1].chunk));
            *error = M_ERR_OK;
        }
        else *error = M_ERR_ALREADY_DEALLOCATED;
    }
    else *error = M_ERR_INVALID_CHUNK;
}


void m_read(int read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {
    if (size_to_read > SIZE_OF_PAGE) {
        *error = M_ERR_INVALID_CHUNK;
        return;
    }
    int counter = 0;
    read_from_id--;
    if (read_from_id <= NUMBER_OF_PAGES) {
        for (int i = 0; i < SIZE_OF_PAGE; i++) {
            if (pages_table[read_from_id].chunk.data[i] != '\0') {
                counter++;
            }
        }
        if (size_to_read <= counter) {
            memcpy(read_to_buffer, pages_table[read_from_id].chunk.data, size_to_read);
            *error = M_ERR_OK;
        }
        else *error = M_ERR_OUT_OF_BOUNDS;
    }
    else *error = M_ERR_OUT_OF_BOUNDS;
}

void initializeChunk(char write_from_buffer[], int size_to_write, int number_of_page) {
    struct Chunk chunk;
    chunk.first_bit_of_chunk = pages_table[number_of_page].first_bit;
    chunk.size = size_to_write;
    strncpy(chunk.data, write_from_buffer, SIZE_OF_PAGE+1);
    pages_table[number_of_page].chunk = chunk;
    pages_table[number_of_page].is_taken = true;
}

void fillSeveralPages(char write_from_buffer[],int* size_to_write, char* first_bitOfFirstPage, int number_of_pages) {
    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        if (pages_table[i].first_bit == first_bitOfFirstPage) {
            for (int j = 0; j < number_of_pages; j++) {
                if (*size_to_write > SIZE_OF_PAGE) {
                    char temp[SIZE_OF_PAGE + 1];
                    strncpy(temp, write_from_buffer, SIZE_OF_PAGE);
                   
                    initializeChunk(temp, SIZE_OF_PAGE, i + j);
                    write_from_buffer = (char*)write_from_buffer + SIZE_OF_PAGE;
                    *size_to_write -= SIZE_OF_PAGE;
                }
                else initializeChunk(write_from_buffer, sizeof(write_from_buffer), i + j);
            }
            break;
        }
    }
}

void m_write(char write_from_buffer[], int size_to_write, m_err_code* error) {
    int number_of_pages;
    if (size_to_write != strlen(write_from_buffer)) {
        *error = M_ERR_INVALID_CHUNK;
        return;
    }

    if (size_to_write % SIZE_OF_PAGE == 0) 
        number_of_pages = size_to_write / SIZE_OF_PAGE; 
    else 
        number_of_pages = size_to_write / SIZE_OF_PAGE + 1;

    int counter = number_of_pages;
    if (number_of_pages > NUMBER_OF_PAGES) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    if (number_of_pages == 1) {
        for (int i = 0; i < NUMBER_OF_PAGES; i++) {
            if (!pages_table[i].is_taken) {
                initializeChunk(write_from_buffer, size_to_write, i);
                *error = M_ERR_OK;
                return;
            }
        }
        *error = M_ERR_OUT_OF_BOUNDS;
    }
    else {
        for (int i = 0; i < NUMBER_OF_PAGES; i++) {

            if (!pages_table[i].is_taken) 
                counter--; 
            else 
                counter = number_of_pages;

            if (counter == 0) {
                char* first_bit_of_first_page = pages_table[i - number_of_pages + 1].first_bit;
                fillSeveralPages(write_from_buffer, &size_to_write, first_bit_of_first_page, number_of_pages);
                *error = M_ERR_OK;
                return;
            }
        }
        *error = M_ERR_OUT_OF_BOUNDS;
    }
}

void listAllBlocks() {
    for (int i = 0; i < NUMBER_OF_PAGES; i++) {
        printf("%d", i + 1);
        printf(". ");
        printf(pages_table[i].chunk.data);
        printf("\n");
    }
}

void m_init() {
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);

    _g_allocator_memory_size = NUMBER_OF_PAGES * SIZE_OF_PAGE;
    _g_allocator_memory = (char*)malloc(_g_allocator_memory_size);
}
