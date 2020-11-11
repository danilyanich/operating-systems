#include <stdlib.h>
#include <string.h>
#include "memory.h"


struct {
    char** pages; // List of pages pointers
    int number_of_pages; // Num of pages
    int size_of_page; // Size of each page
    int **bytes_allocated; // Usage matrix

} g_allocator_memory;


m_id m_malloc(int size_of_chunk, m_err_code *error) {
    int *result_id;

    // Iteration in pages
    for (int i = 0; i<g_allocator_memory.number_of_pages; i++) {

        // Iteration in space of page
        int free_counter = 0;
        for (int t = 0; t < g_allocator_memory.size_of_page; t++) {

            if (g_allocator_memory.bytes_allocated[i][t] == 0) {
                free_counter++;

                // if there is enough space
                if (free_counter >= size_of_chunk) {
                    int start = t-free_counter+1;

                    for (int j = start; j <= t; j++) {
                        g_allocator_memory.bytes_allocated[i][j] = 1;
                    }

                    result_id = (int *) (g_allocator_memory.pages[i] + start);

                    *error = M_ERR_OK;
                    return result_id;

                }
            } else {
                free_counter = 0;
            }

        }
    }

    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
    return NULL;
}


void m_free(m_id ptr, int size_to_free, int page, m_err_code *error) {
    if(ptr==NULL){
        *error = M_ERR_ALREADY_DEALLOCATED;
        return;
    }
    int start = (int)(ptr - (int)g_allocator_memory.pages[page]);
    int end = start + size_to_free;

    for (int i = start; i <= end; ++i) {
        g_allocator_memory.bytes_allocated[page][i] = 0;
    }
    ptr = NULL;
    *error = M_ERR_OK;
}


void m_read(m_id read_from_id, void *read_to_buffer, int size_to_read, m_err_code *error) {
    memcpy(read_to_buffer, read_from_id, size_to_read);
    *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void *write_from_buffer, int size_to_write, m_err_code *error) {
    memcpy(write_to_id, write_from_buffer, size_to_write);
    *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
    //init sandbox
    g_allocator_memory.size_of_page = size_of_page;
    g_allocator_memory.number_of_pages = number_of_pages;

    g_allocator_memory.pages = (char**)malloc(number_of_pages*sizeof(char*));
    g_allocator_memory.bytes_allocated = (int**)malloc(number_of_pages* sizeof(int*));

    for (int i = 0; i < number_of_pages; ++i) {

        g_allocator_memory.pages[i] = (char*) malloc(size_of_page*sizeof(char));
        g_allocator_memory.bytes_allocated[i] = (int*)malloc(size_of_page*sizeof(int));

        for (int j = 0; j < size_of_page; ++j) {
            g_allocator_memory.bytes_allocated[i][j] = 0;
        }
    }
}
