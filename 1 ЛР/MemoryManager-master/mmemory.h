#ifndef MMEMORY_H_INCLUDED
#define MMEMORY_H_INCLUDED

#include <stdlib.h>
#include <stdbool.h>



#endif // MMEMORY_H_INCLUDED

typedef char* VA;				// Тип описывающий адрес блока 

MMEMORY_API int _malloc (VA* ptr, size_t szBlock);

MMEMORY_API int _free (VA ptr);

MMEMORY_API int _read (VA ptr, void* pBuffer, size_t szBuffer);

MMEMORY_API int _write (VA ptr, void* pBuffer, size_t szBuffer);

MMEMORY_API int _init (int n, int szPage);

MMEMORY_API void _printBuffer(VA ptr, size_t size);