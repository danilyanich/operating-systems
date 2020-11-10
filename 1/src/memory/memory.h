#ifndef MEMORY_H
#define MEMORY_H

#include <stdbool.h>

#define M_ERR_OK 0 // Everything is ok
#define M_ERR_ALLOCATION_OUT_OF_MEMORY 1 // Not enough memory for allocation
#define M_ERR_ALREADY_DEALLOCATED 2 // The chunk was already deallocated
#define M_ERR_INVALID_CHUNK 3 // The chunk is invalid, the operation did not succeed
#define M_ERR_OUT_OF_BOUNDS 4 // The read/write operation out of bounds
#define NUMBER_OF_PAGES 10
#define SIZE_OF_PAGE 5

struct Chunk{
    char* first_bit_of_chunk;
    int size;
    char data[6];
};

struct Page{
    bool is_taken;
    char* first_bit;
    struct Chunk chunk;
};

struct Page pages_table[10];

typedef int m_err_code; // Error code of sandbox memory
typedef void* m_id; // Identifier of sandbox memory chunk


// Allocates a chunk in sandbox memory
// @param size_of_chunk Desired size in bytes to be allocated in sandbox memory
// @param error_code [out] M_ERR_OK, M_ERR_ALLOCATION_OUT_OF_MEMORY
// @return An identifier for newly allocated chunk
void m_malloc(m_err_code *error);
void listAllBlocks();

// Deallocates a chunk in sandbox memory
// @param chunk_id Chunk identifier
// @param error_code [out] M_ERR_OK, M_ERR_ALREADY_DEALLOCATED, M_ERR_INVALID_CHUNK
void m_free(int ptr, m_err_code* error_code);


// Reads from chunk to a buffer
// @param read_from_id The chunk to read from
// @param read_to_buffer [out] The buffer to store data into
// @param size_to_read Size of data in bytes to read from chunk
// @param error_code [out] M_ERR_OK, M_ERR_INVALID_CHUNK, M_ERR_OUT_OF_BOUNDS
void m_read(int read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error_code);


// Writes from buffer to a chunk
// @param write_to_id The chunk to store data into
// @param write_from_buffer The buffer to read from
// @param size_to_write Size of data in bytes to be stored into chunk
// @param error_code [out] M_ERR_OK, M_ERR_INVALID_CHUNK, M_ERR_OUT_OF_BOUNDS
void m_write(void* write_from_buffer, int size_to_write, m_err_code* error_code);


// Initializes sandbox memory allocator. Usually it is number_of_pages*size_of_page.
// @param number_of_pages Number of pages to allocate
// @param size_of_page Size of the page
void m_init();


#endif /* MEMORY_H */
