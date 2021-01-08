#include <stdlib.h>
#include <string.h>

#include "memory.h"

memory* _g_allocator_memory;

int _g_bytes_allocated = 0;

m_id m_malloc(int size_of_segment, m_err_code* error) {
  if (_g_bytes_allocated + size_of_segment > _g_allocator_memory->size) {
    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
    return NULL;
  }

  segment* new_segment = malloc(sizeof(segment));
  new_segment->size = size_of_segment;
  new_segment->next = NULL;
  new_segment->prev = NULL;
  new_segment->isFilled = false;

  if (_g_allocator_memory->next == NULL) { 
      _g_allocator_memory->next = new_segment;
      new_segment->data = _g_allocator_memory->memory;
  }
  else {
      segment* last_segment = _g_allocator_memory->next;
      while (true) {
          if (last_segment->next == NULL ||
              (last_segment->next->isFilled == false && last_segment->next->size >= new_segment->size)) {
              break;
          }
          last_segment = last_segment->next;
      }

      if (last_segment->data + last_segment->size + size_of_segment >= _g_allocator_memory->memory + _g_allocator_memory->size) {
          *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
          return NULL;
      }

      
      new_segment->prev = last_segment;
      new_segment->data = last_segment->data + last_segment->size;

      if (last_segment->next != NULL && last_segment->next->isFilled == false) {
          segment* empty_segment = malloc(sizeof(segment));
          empty_segment->next = last_segment->next->next;
          empty_segment->prev = new_segment;
          empty_segment->size = last_segment->next->size;
          empty_segment->data = last_segment->next->data + new_segment->size;
          empty_segment->isFilled = false;

          new_segment->next = empty_segment;
          new_segment->data = last_segment->next->data;
      }      

      last_segment->next = new_segment;
  }

  if (new_segment->data == NULL) {
      *error = M_ERR_ALLOCATION_NOT_SUCCEED;
      new_segment = NULL;
      return NULL;
  }

  *error = M_ERR_OK;
  _g_bytes_allocated += size_of_segment;
  return new_segment;
}


void m_free(m_id ptr, m_err_code* error) {
    ptr->isFilled = false;

    _g_bytes_allocated -= ptr->size;
    
    *error = M_ERR_OK;
}


void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {
  memcpy(read_to_buffer, read_from_id->data, size_to_read);
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
  memcpy(write_to_id->data, write_from_buffer, size_to_write);
  write_to_id->isFilled = true;
  *error = M_ERR_OK;
}


void m_init(int size_of_memory, m_err_code* error) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory = calloc(1, sizeof(memory)); 
  _g_allocator_memory->size = size_of_memory;
  _g_allocator_memory->memory = calloc(1, size_of_memory);

  if (_g_allocator_memory == NULL) {
      *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
      return NULL;
  }

  _g_bytes_allocated = 0;
  *error = M_ERR_OK;
}