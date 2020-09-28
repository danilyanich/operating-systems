#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "memory/memory.h"

int main() {
    int error;
    m_init(500);
    chunk *first = m_malloc(34, &error);
    assert(error == M_ERR_OK);

    chunk *second = m_malloc(36, &error);
    assert(error == M_ERR_OK);

    chunk *third = m_malloc(23, &error);
    assert(error == M_ERR_OK);

    chunk *fourth = m_malloc(64, &error);
    assert(error == M_ERR_OK);

    m_err_code success = m_free(fourth);
    assert(success == M_ERR_OK);

    chunk *chunk_1 = m_malloc(13, &error);
    if (error != M_ERR_OK) abort();

    chunk *chunk_2 = m_malloc(20, &error);
    if (error != M_ERR_OK) abort();

    chunk *chunk_3 = m_malloc(50, &error);
    if (error != M_ERR_OK) abort();

    error = m_write(chunk_1, "Hello World!", 13);
    if (error != M_ERR_OK) abort();

    error = m_write(chunk_2, "Operating Systems", 18);
    if (error != M_ERR_OK) abort();

    error = m_write(chunk_3, "Super dumb memory allocator", 28);
    if (error != M_ERR_OK) abort();

    char* buffer = malloc(50);

    error = m_read(chunk_1, buffer, 13);
    if (error != M_ERR_OK) abort();
    printf("%s\n", buffer);

    error = m_read(chunk_2, buffer, 18);
    if (error != M_ERR_OK) abort();
    printf("%s\n", buffer);

    error = m_read(chunk_3, buffer, 28);
    if (error != M_ERR_OK) abort();
    printf("%s\n", buffer);

    error = m_free(chunk_1);
    if (error != M_ERR_OK) abort();

    error = m_free(chunk_2);
    if (error != M_ERR_OK) abort();

    error = m_free(chunk_3);
    if (error != M_ERR_OK) abort();
}