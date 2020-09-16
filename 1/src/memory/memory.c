#include <stdlib.h>
#include <string.h>

#include "memory.h"


char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;


m_err_code m_malloc(
  m_id* output_id,
  int size_of_chunk
) {
  if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) return M_ERR_OUT_OF_MEMORY;

  _g_bytes_allocated += size_of_chunk;
  *output_id = (_g_allocator_memory + _g_bytes_allocated);

  return M_ERR_OK;
}


m_err_code m_free(
  m_id ptr
) {
  return M_ERR_OK;
}


m_err_code m_read(
  m_id read_from_id,
  void* read_to_buffer,
  int size_to_read
) {
  memcpy(read_to_buffer, read_from_id, size_to_read);

  return M_ERR_OK;
}


m_err_code m_write(
  m_id write_to_id,
  void* write_from_buffer,
  int size_to_write
) {
  memcpy(write_to_id, write_from_buffer, size_to_write);

  return M_ERR_OK;
}


m_err_code m_init(
  int number_of_pages,
  int size_of_page
) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;

  return M_ERR_OK;
}
