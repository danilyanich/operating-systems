//
//  main.c
//  memory-manager
//
//  Created by Kiril Malenchik on 10/10/20.
//

#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"


void errorAbort(int error_code);

int main(int argc, const char * argv[]) {
    m_init(1,500);
    int error_code;
    
    char buffer[50];
    
    m_id chunk_1 = m_malloc(501, &error_code);
    errorAbort(error_code);
    
    m_free(chunk_1, &error_code);
    errorAbort(error_code);

    m_free(chunk_1, &error_code);
    errorAbort(error_code);
    
    m_read(chunk_1, buffer, 13, &error_code);
    errorAbort(error_code);
    printf("%s\n", buffer);
    
    m_write(chunk_1, "Hello World!", 13, &error_code);
    errorAbort(error_code);

    
//    m_read(chunk_1 + 1, buffer, 13, &error_code);
//    errorAbort(error_code);
    
    m_id chunk_2 = m_malloc(20, &error_code);
    errorAbort(error_code);
    
    m_write(chunk_2, "Operating Systems", 18, &error_code);
    errorAbort(error_code);
    
    m_read(chunk_2, buffer, 18, &error_code);
    errorAbort(error_code);
    printf("%s\n", buffer);
    
    m_free(chunk_2, &error_code);
    errorAbort(error_code);  

    m_id chunk_3 = m_malloc(50, &error_code);
    errorAbort(error_code);

    m_write(chunk_3, "Super dumb memory allocator", 28, &error_code);
    errorAbort(error_code);

    m_read(chunk_3, buffer, 28, &error_code);
    errorAbort(error_code);
    printf("%s\n", buffer);

    m_free(chunk_3, &error_code);
    errorAbort(error_code);
}

void errorAbort(int error_code){
    
    if (error_code != M_ERR_OK){
        printf("Error: %d\n", error_code);
    }
}
