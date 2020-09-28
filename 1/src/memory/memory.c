#include <stdlib.h>
#include <string.h>

#include "memory.h"

memory* _g_allocator_memory;

int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

m_id m_malloc(int size_of_segment, m_err_code* error) {
  if (_g_bytes_allocated + size_of_segment > _g_allocator_memory_size) {
    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
    return NULL;
  }

  _g_bytes_allocated += size_of_segment;

  segment* new_segment = _g_allocator_memory->next;
  segment* prev_segment = _g_allocator_memory->next;

  while (new_segment != NULL) {
      prev_segment = new_segment;
      new_segment = new_segment->next;
  }

  new_segment = calloc(1, sizeof(segment));

  if (_g_allocator_memory->next == NULL) _g_allocator_memory->next = new_segment;

  if (prev_segment != NULL && prev_segment->next == NULL) prev_segment->next = new_segment;

  new_segment->size = size_of_segment;
  new_segment->data = calloc(1, size_of_segment);
  new_segment->prev = prev_segment;

  if (new_segment->data == NULL) {
      *error = M_ERR_ALLOCATION_NOT_SUCCEED;
      new_segment = NULL;
      return NULL;
  }

  *error = M_ERR_OK;
  return new_segment;
}


void m_free(m_id ptr, m_err_code* error) {
    if (ptr->prev != NULL) {
        ptr->prev->next = ptr->next;
    }
    else {
        _g_allocator_memory->next = ptr->next;
        if (ptr->next != NULL) ptr->next->prev = NULL;
    }

    _g_bytes_allocated -= ptr->size;
    free(ptr);
    
    *error = M_ERR_OK;
}


void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {
  memcpy(read_to_buffer, read_from_id->data, size_to_read);
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
  memcpy(write_to_id->data, write_from_buffer, size_to_write);
  *error = M_ERR_OK;
}


void m_init(int size_of_memory) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = size_of_memory;
  _g_allocator_memory = calloc(1, sizeof(memory));

  if (_g_allocator_memory == NULL) {
      return M_ERR_ALLOCATION_OUT_OF_MEMORY;
  }

  _g_bytes_allocated = 0;
}
