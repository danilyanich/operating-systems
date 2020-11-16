#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
    char buffer[50];
    m_init(10, 5);

    int error_code;
    m_id chunk_1 = m_malloc(11, error_code);
    m_write(chunk_1, "1234", 4, error_code);
    listAllBlocks();
////    if (error_code != M_ERR_OK) abort();
//
//    m_write("1234", 4, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
//    m_write("alinaliza", 9, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
//    m_free(2, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
//    m_write("iloveyou", 8, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
////    m_read(5, buffer, 5, &error_code);
////    if (error_code != M_ERR_OK) abort();
////    else printf("%s\n", buffer);
//
//    m_write("iloveoperations", 15, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
//    m_write("hello", 5, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
////    m_read(4, buffer, 5, &error_code);
////    if (error_code != M_ERR_OK) abort();
////    else printf("%s\n", buffer);
//
////    m_read(11, buffer, 5, &error_code);
////    if (error_code != M_ERR_OK) abort();
////    else printf("%s\n", buffer);
//
////    m_free(2, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
////    m_write("osi", 2, &error_code);
////    if (error_code != M_ERR_OK) abort();
//
////    m_read(4, buffer, 6, &error_code);
////    if (error_code != M_ERR_OK) abort();
////    else printf("%s\n", buffer);

    listAllBlocks();
}
