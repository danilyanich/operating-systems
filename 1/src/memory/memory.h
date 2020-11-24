#ifndef MEMORY_H
#define MEMORY_H

#define M_ERR_OK 0 // Everything is ok
#define M_ERR_ALLOCATION_OUT_OF_MEMORY 1 // Not enough memory for allocation
#define M_ERR_ALREADY_DEALLOCATED 2 // The chunk was already deallocated
#define M_ERR_INVALID_CHUNK 3 // The chunk is invalid, the operation did not succeed
#define M_ERR_OUT_OF_BOUNDS 4 // The read/write operation out of bounds
#define M_WARN_NO_IN_CACHE 5 // No specific chunk in cache

typedef int m_err_code; // Error code of sandbox memory
typedef int* segment_addr;
typedef struct chunk chunk;


// Allocates a chunk in sandbox memory
// @param size_of_chunk Desired size in bytes to be allocated in sandbox memory
// @param error_code [out] M_ERR_OK, M_ERR_ALLOCATION_OUT_OF_MEMORY
// @return An identifier for newly allocated chunk
segment_addr m_malloc(int size_of_chunk, m_err_code *error_code);


// Deallocates a chunk in sandbox memory
// @param chunk_id Chunk identifier
// @return error_code M_ERR_OK, M_ERR_ALREADY_DEALLOCATED, M_ERR_INVALID_CHUNK
m_err_code m_free(segment_addr chunk_id);


// Reads from chunk to a buffer
// @param read_from_id The chunk to read from
// @param read_to_buffer [out] The buffer to store data into
// @param size_to_read Size of data in bytes to read from chunk
// @return error_code M_ERR_OK, M_ERR_INVALID_CHUNK, M_ERR_OUT_OF_BOUNDS
m_err_code m_read(segment_addr read_from_id, void *read_to_buffer, int size_to_read);


// Writes from buffer to a chunk
// @param write_to_id The chunk to store data into
// @param write_from_buffer The buffer to read from
// @param size_to_write Size of data in bytes to be stored into chunk
// @return error_code M_ERR_OK, M_ERR_INVALID_CHUNK, M_ERR_OUT_OF_BOUNDS
m_err_code m_write(segment_addr write_to_id, void *write_from_buffer, int size_to_write);


// Initializes sandbox memory allocator. Usually it is number_of_pages*size_of_page.
// @param size Size of the allocated memory
void m_init(int size);


#endif /* MEMORY_H */