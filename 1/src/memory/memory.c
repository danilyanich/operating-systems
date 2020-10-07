#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"


struct {
    char* pages[MAX_PAGES]; // List of pages pointers
    int pages_num;
    int page_size; // Size of each page
    int bytes_allocated[MAX_PAGES][MAX_PAGE_SIZE]; // Allocated bytes of each page

} _g_allocator_memory;


m_id m_malloc(int size_of_chunk, m_err_code* error) {
    bool space_is_found = false;
    int *result_id;

    // Iteration in pages
    for (int i = 0; i<_g_allocator_memory.pages_num; i++) {

        // Iteration in space of page
        int free_counter = 0;
        for (int t = 0; t < _g_allocator_memory.page_size; t++) {

            if (_g_allocator_memory.bytes_allocated[i][t] == 0) {
                free_counter++;

                // if there is enough space
                if (free_counter >= size_of_chunk) {
                    space_is_found = true;
                    int start = t-free_counter+1;

                    for (int j = start; j <= t; j++) {
                        _g_allocator_memory.bytes_allocated[i][j] = 1;
                    }

                    result_id = _g_allocator_memory.pages[i] + start;
                    goto massive_break;

                }
            } else {
                free_counter = 0;
            }

        }
    }

    massive_break:



    if (space_is_found == false) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }


    *error = M_ERR_OK;
    return result_id;
}


void m_free(m_id ptr, int size_to_free, int page, m_err_code* error) {
    int start = (int)(ptr - (int)_g_allocator_memory.pages[page]);
    int end = start + size_to_free;
    for (int i = start; i <= end; ++i) {
        _g_allocator_memory.bytes_allocated[page][i] = 0;
    }
    *error = M_ERR_OK;
}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
  memcpy(read_to_buffer, read_from_id, size_to_read);
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
  memcpy(write_to_id, write_from_buffer, size_to_write);
  *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
    // Init sandbox memory
    _g_allocator_memory.page_size = size_of_page;
    _g_allocator_memory.pages_num = number_of_pages;
    for (int i = 0; i<number_of_pages; i++)
    {
        // Fill usage matrix with zeros
        _g_allocator_memory.pages[i] = malloc(size_of_page);
        for (int t = 0; t<size_of_page; t++){
            _g_allocator_memory.bytes_allocated[i][t] = 0;
        }
    }
}
