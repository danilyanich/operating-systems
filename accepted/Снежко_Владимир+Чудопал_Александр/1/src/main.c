#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {

    m_init(5, 10, 10);

    int error_code;

    m_id chunk_1 = m_malloc(11, error_code);
    m_id chunk_2 = m_malloc(12, error_code);
    m_id chunk_3 = m_malloc(13, error_code);

    m_free(chunk_2, error_code); 

    printf("This is chunk_2 %p\n", chunk_2);
    printf("This is chunk_2 -> next %p\n", chunk_2 -> next);

    m_id chunk_2_1 = m_malloc(4, error_code);
    m_id chunk_2_2 = m_malloc(8, error_code);

    m_id chunk_4 = m_malloc(14, error_code);

    printf("chunk_1 - %p\n", chunk_1);
    printf("chunk_1 -> next - %p\n", chunk_1->next);
    printf("chunk_2_1 - %p\n", chunk_2_1);
    printf("chunk_2_1 -> next - %p\n", chunk_2_1->next);
    printf("chunk_2_2 - %p\n", chunk_2_2);
    printf("chunk_2_2 -> next - %p\n", chunk_2_2->next);
    printf("chunk_3 - %p\n", chunk_3);
    printf("chunk_3 -> next - %p\n", chunk_3->next);
}
