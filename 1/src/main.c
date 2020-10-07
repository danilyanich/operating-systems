#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
    m_init(4, 50);

    int error_code;

    m_id chunk_1 = m_malloc(13, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_1, "Hello World!", 13, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_id chunk_2 = m_malloc(23, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_2, "Second chunk, eaya boy", 23, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_id chunk_3 = m_malloc(11, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_3, "damn hommy", 11, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_free(chunk_2, 23, 0, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_id chunk_4 = m_malloc(5, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_4, "laba", 5, &error_code);
    if (error_code != M_ERR_OK) abort();

    char buffer[60];

    m_read(chunk_1, buffer, 13, &error_code);
    if (error_code != M_ERR_OK) abort();
    printf("%s\n", buffer);


    m_read(chunk_3, buffer, 11, &error_code);
    if (error_code != M_ERR_OK) abort();
    printf("%s\n", buffer);

    m_read(chunk_4, buffer, 5, &error_code);
    if (error_code != M_ERR_OK) abort();
    printf("%s\n", buffer);


}