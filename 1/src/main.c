#include "memory/memory.h"
#include <stdlib.h>
#include "assert.h"
#include <string.h>

void test_str_alloc(char* data) {
    m_id ptr = NULL;
    m_id* ptr2 = &ptr;
    
    m_malloc(ptr2, strlen(data));
    m_write(*ptr2, data, strlen(data));

    void* resData = malloc(strlen(data));
    m_read(*ptr2, resData, strlen(data));
    assert(strcmp((char*)resData, data));

    m_free(*ptr2);

    void* emptyData = malloc(strlen(data));
    int res_code = m_read(*ptr2, emptyData, strlen(data));
    assert(res_code == M_ERR_BAD_PARAMS);
}

int main(int argc, char* argv[]) {

    int res = m_init(3, 10);
    assert(res == 0);

    test_str_alloc((char*)"Hello");
    test_str_alloc((char*)"world!");
    test_str_alloc((char*)"Test");
    test_str_alloc((char*)"word");

    m_clean();

    printf("I'm OK!");

    return 0;
}