#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
    m_init(1, 500);

    int error_code;

    m_malloc(100, &error_code);
    if (error_code != M_ERR_OK) printf("Error: %d", error_code);

    m_id chunk_3 = m_malloc(200, &error_code);
    if (error_code != M_ERR_OK) printf("Error: %d", error_code);

    m_malloc(200, &error_code);
    if (error_code != M_ERR_OK) printf("Error: %d", error_code);

    m_free(chunk_3, &error_code);
    if (error_code != M_ERR_OK) printf("Error: %d", error_code);

    m_malloc(200, &error_code);
    if (error_code != M_ERR_OK) printf("Error2: %d", error_code);

//    m_malloc(500, &error_code);
//    if (error_code != M_ERR_OK) printf("Error: %d", error_code);
//
//    m_write(chunk_2,"qqqq",5,&error_code);
//    if (error_code != M_ERR_OK) printf("Error: %d", error_code);
//
//    char buffer[50];
//
//    m_read(chunk_2, buffer, 5, &error_code);
//    if (error_code != M_ERR_OK) printf("Error: %d", error_code);
//    else printf("%s\n", buffer);
//
//    m_free(chunk_2, &error_code);
//    if (error_code != M_ERR_OK) printf("Error: %d", error_code);
//    m_free(NULL, &error_code);


}
