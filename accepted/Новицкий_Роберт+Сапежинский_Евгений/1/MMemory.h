#ifndef UNTITLED_MMEMORY_H
#define UNTITLED_MMEMORY_H

#include<stdio.h>

typedef char *VA;

#define SUCCESS 0
#define ERR_BAD_PARAMS -1
#define ERR_MEM -2
#define ERR_UNKNOWN 1


int _malloc(VA *ptr, size_t szBlock);

int _free(VA ptr);

int _read(VA ptr, void *pBuffer, size_t szBuffer);

int _write(VA ptr, void *pBuffer, size_t szBuffer);

int _init_(int n, int szPage);

int _clean();
#endif
