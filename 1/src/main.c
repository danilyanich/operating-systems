#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
    m_init(4, 50);

    int error_code;

    // Page size: 50
    m_id chunk_1 = m_malloc(13, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_1, "Hello World!", 13, &error_code);
    if (error_code != M_ERR_OK) abort();

    // Allocating Second Chunck with size 23 - "Second chunk, eaya boy"
    m_id chunk_2 = m_malloc(23, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_2, "Second chunk, eaya boy", 23, &error_code);
    if (error_code != M_ERR_OK) abort();

    // Allocating Third Chunck with size 11 - "damn hommy"
    // 47/50 bytes of first page are used, there is no memory for next chunk with size 5
    m_id chunk_3 = m_malloc(11, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_3, "damn hommy", 11, &error_code);
    if (error_code != M_ERR_OK) abort();

    // To test free function, we drop second chunk
    m_free(chunk_2, 23, 0, &error_code);
    if (error_code != M_ERR_OK) abort();

    // Allocating Forth Chunck with size 5, which uses freed space of Second Chunk
    m_id chunk_4 = m_malloc(5, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_4, "laba", 5, &error_code);
    if (error_code != M_ERR_OK) abort();

    // Allocating Fifth Chunck with size 40, to show page separation; Memory for this chunk will be allocated from second page
    m_id chunk_5 = m_malloc(40, &error_code);
    if (error_code != M_ERR_OK) abort();
    m_write(chunk_5, "Paging is working))", 40, &error_code);
    if (error_code != M_ERR_OK) abort();

    // Printing chunks from memory
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

    m_read(chunk_5, buffer, 40, &error_code);
    if (error_code != M_ERR_OK) abort();
    printf("%s\n", buffer);


}