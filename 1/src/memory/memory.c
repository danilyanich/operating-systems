#include <stdlib.h>
#include <string.h>

#include "memory.h"


char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;


struct block* top = NULL;
struct block* begin = NULL;

m_id m_malloc(int size_of_chunk, m_err_code* error) {

    if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    struct block* tmp = malloc(sizeof(*tmp));
    struct block* current_block = begin;
    int result = 0;

    if (begin == NULL) {
        begin = tmp;
        tmp->memory = _g_allocator_memory + size_of_chunk;

    }
    else {
        result = insertBlock(tmp, current_block, size_of_chunk);
    }

    if (result) {
        printf("\n%d, %d ", (int)_g_allocator_memory, size_of_chunk);
        printf("\n%d ", (int)tmp->memory);
        *error = M_ERR_OK;
        return (m_id)tmp;
    }
   
    *error = M_ERR_OK;
    return (m_id)tmp;
}


void m_free(m_id ptr, m_err_code* error) {
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
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;
}
