
#include <stdbool.h>
#include <stddef.h>

#define SUCCESS 0
#define ERR_UNKNOWN 1
#define ERR_INPUT -1
#define ERR_SEG_FAULT -2

typedef char *MemAddr; // Тип - Адрес блока

int _malloc(MemAddr *addr, size_t blockSize);

int _free(MemAddr addr);

int _read(MemAddr addr, void *bufferStart, size_t bufferSize);

int _write(MemAddr addr, void *bufferStart, size_t bufferSize);

int _init(int amountOfPages, int pageSize);
