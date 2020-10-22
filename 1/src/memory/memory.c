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

  _g_bytes_allocated += size_of_chunk;

  
  struct block* tmp = malloc(sizeof(*tmp));

  tmp->start = _g_allocator_memory + _g_bytes_allocated;
  tmp->size = size_of_chunk;
  tmp->prev = top;
  tmp->next = NULL;
  top = tmp;

  if (tmp->prev != NULL) 
      tmp->prev->next = tmp;
  if (begin == NULL)
      begin = tmp;

  *error = M_ERR_OK;
  return (m_id)tmp;

}


void m_free(m_id ptr, m_err_code* error) {

    struct block* ptr_new = ptr;

    if (ptr == NULL) {
        *error = M_ERR_ALREADY_DEALLOCATED;
    }

    struct block* tmp;
    if (ptr_new->start == top->start)
    {
        tmp = top;
        top = ptr_new->prev;
        free(tmp);

    }
    else
        free(ptr_new);
  *error = M_ERR_OK;
}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
    struct block* ptr_new = read_from_id;

    if (size_to_read > ptr_new->size) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    if (&read_from_id != NULL && ptr_new->start != NULL)
    {
        memcpy(read_to_buffer, ptr_new->start, size_to_read);
    }
    else {
        *error = M_ERR_INVALID_CHUNK;
    }
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    struct block* ptr_new = write_to_id;

    if (write_to_id == NULL) {
        *error = M_ERR_INVALID_CHUNK;
    }
   
    if (size_to_write > ptr_new->size) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    memcpy(ptr_new->start, write_from_buffer, size_to_write);
     *error = M_ERR_OK;
}


void  m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;

  
}
