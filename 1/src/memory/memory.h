#ifndef MEMORY_H
#define MEMORY_H

// Return codes
#define M_ERR_OK 0
#define M_ERR_INVALID_ARGUMENTS 1
#define M_ERR_OUT_OF_MEMORY 2
#define M_ERR_OUT_OF_BOUNDS 3


typedef int m_err_code; // error code of sandbox memory
typedef void* m_id; // identifier of sandbox memory chunk


// Allocates a chunk in sandbox memory
// returns M_ERR_OK, M_ERR_INVALID_ARGUMENTS, M_ERR_OUT_OF_MEMORY
m_err_code m_malloc(
  m_id* output_id, // a pointer to chunk
  int size_of_chunk // size of the chunk in bytes
);


// Deallocates a chunk in sandbox memory
// returns M_ERR_OK, M_ERR_INVALID_ARGUMENTS
m_err_code m_free(
  m_id ptr // chunk identifier
);


// Reads from chunk to a buffer
// returns M_ERR_OK, M_ERR_INVALID_ARGUMENTS, M_ERR_OUT_OF_BOUNDS
m_err_code m_read(
  m_id read_from_id, // chunk identifier to read from
  void* read_to_buffer, // pointer to buffer(real memory address) where to read into
  int size_to_read // size to read from chunk
);


// Writes to chunk from a buffer
// returns M_ERR_OK, M_ERR_INVALID_ARGUMENTS, M_ERR_OUT_OF_BOUNDS
m_err_code m_write(
  m_id write_to_id, // chunk identifier to write into
  void* write_from_buffer, // pointer to buffer(real memory address) where to write from
  int size_to_write // size to write into chunk
);


// Initializes sandbox memory allocator
// returns M_ERR_OK, M_ERR_INVALID_ARGUMENTS
m_err_code m_init(
  int number_of_pages,
  int size_of_page
);


#endif /* MEMORY_H */
