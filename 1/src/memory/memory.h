#ifndef MEMORY_H
#define MEMORY_H

#define M_ERR_OK 0 // Everything is ok
#define M_ERR_ALLOCATION_OUT_OF_MEMORY 1 // Not enough memory for allocation
#define M_ERR_ALREADY_DEALLOCATED 2 // The chunk was already deallocated
#define M_ERR_INVALID_CHUNK 3 // The chunk is invalid, the operation did not succeed
#define M_ERR_OUT_OF_BOUNDS 4 // The read/write operation out of bounds

typedef char* VA;

int _init(int n, int szPage);
int _malloc(VA * ptr, size_t szBlock);
int _free(VA ptr);
int _read(VA ptr, void* pBuffer, size_t szBuffer);
int _write(VA ptr, void* pBuffer, size_t szBuffer);

#endif /* MEMORY_H */
