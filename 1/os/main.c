
#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

int main(int argc, char** argv) {
    char buffer[50];
    m_init();

    int error_code;
    m_malloc(&error_code);

    char a[] = "1234";
    m_write(a, 4, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_write(a, 4, &error_code);
    if (error_code != M_ERR_OK) abort();

    char b[] = "alinaliza";
    m_write(b, 9, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_free(2, &error_code);
    if (error_code != M_ERR_OK) abort();

    char c[] = "iloveyou";
    m_write(c, 8, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_read(5, buffer, 5, &error_code);
    if (error_code != M_ERR_OK) abort();
    else printf("%s\n", buffer);

    char d[] = "iloveoperations";
    m_write(d, 15, &error_code);
    if (error_code != M_ERR_OK) abort();

    char r[] = "hello";
    m_write(r, 5, &error_code);
    if (error_code != M_ERR_OK) abort();

    //    m_read(4, buffer, 5, &error_code);
    //    if (error_code != M_ERR_OK) abort();
    //    else printf("%s\n", buffer);

    //    m_read(11, buffer, 5, &error_code);
    //    if (error_code != M_ERR_OK) abort();
    //    else printf("%s\n", buffer);

    //    m_free(2, &error_code);
    //    if (error_code != M_ERR_OK) abort();

    //    m_write("osi", 2, &error_code);
    //    if (error_code != M_ERR_OK) abort();

    //    m_read(4, buffer, 6, &error_code);
    //    if (error_code != M_ERR_OK) abort();
    //    else printf("%s\n", buffer);

    listAllBlocks();
}
