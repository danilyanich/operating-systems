#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "unit_tests.h"
#include "../header/util.h"

#define SUCCESS "0(Success)"
#define FAIL_UNEXP "1(Unexpected error)"
#define FAIL_WR_INP "-1(Wrong input parameters)"
#define FAIL_SF "-2(Access beyoud the segment)"

memory_nodel* init_test_env(size_t sz, const char *buf, size_t szBuf, size_t count, ...) {
    memory_nodel *mem = calloc(1, sizeof(memory_nodel));
    mem->memory_size = sz;
    mem->physical_address = (size_t) calloc(mem->memory_size, 1);

    // Copying data from buf
    for (int i = 0; i < szBuf; i++) {
        *((char *) (mem->physical_address + i)) = buf[i];
    }

    // Creating segment table
    va_list lengths;
            va_start(lengths, count);

    mem->first_segment = calloc(1, sizeof(segment_queue));

    segment_queue *s = mem->first_segment;
    size_t len;
    VA va = 0;
    for (int i = 0; i < count; i++) {
        len = va_arg(lengths, size_t);
        s->segment_length = len;

        va = (VA) (va + len);

        s->next = calloc(1, sizeof(segment_queue));
        s->next->va = va;

        s = s->next;
    }

            va_end(lengths);

    init_test_memory(mem);

    return mem;
}

char* code_to_str(const int code) {
    switch (code) {
        case RC_SUCCESS:
            return SUCCESS;
        case RC_ERR_U:
            return FAIL_UNEXP;
        case RC_ERR_INPUT:
            return FAIL_WR_INP;
        case RC_ERR_SF:
            return FAIL_SF;
    }
    return NULL;
}

typedef struct {
    char *name;
    int actual_code;
    int return_code;
    int n;
    int szPage;
    size_t szBlock;
    VA ptr;
    char *pBuffer;
    size_t szBuffer;
} TC;

void test_init() {
    printf("\n_init test:\n");
    TC tc[] = {
            {.name = "10 element", .n = 10, .szPage = 1},
            {.name = "100 elements", .n = 100, .szPage = 1},
            {.name = "1000 elements", .n = 1000, .szPage = 1},
    };
    for (int i = 0; i < sizeof(tc) / sizeof(TC); i++) {
        _T_START;
        tc[i].actual_code = _init(tc[i].n, tc[i].szPage);
        _T_STOP;
        if (tc[i].actual_code == tc[i].return_code) {
            printf("-- %s\n\
                  \r   PASS(%fsec)\n",
                   tc[i].name, _T_DIFF);
            continue;
        }
    }
}

void test_malloc() {
    printf("\n_malloc test:\n");

    TC tc[] = {
            {.name = "1 byte", .szBlock = 1},
            {.name = "10 byte", .szBlock = 10},
            {.name = "100 byte", .szBlock = 100},
    };

    init_test_env(111, NULL, 0, 0);

    for (int i = 0; i < sizeof(tc) / sizeof(TC); i++) {
        _T_START;
        tc[i].actual_code = _malloc(&tc[i].ptr, tc[i].szBlock);
        _T_STOP;
        if (tc[i].actual_code == tc[i].return_code) {
            printf("-- %s\n\
                  \r   PASS(%fsec)\n",
                   tc[i].name, _T_DIFF);
            continue;
        }
    }
}

void test_write() {
    printf("\n_write test:\n");

    TC tc[] = {
            {.name = "0 size buffer", .return_code = RC_ERR_INPUT, .ptr = (VA) 0, .pBuffer = (char *) 1, .szBuffer = 0},
            {.name = "Ptr out of range", .return_code = RC_ERR_SF, .ptr = (VA) 999, .pBuffer = (char *) 1, .szBuffer = 1},
    };
    init_test_env(3, NULL, 0, 1, 3);
    for (int i = 0; i < sizeof(tc) / sizeof(TC); i++) {
        _T_START;
        tc[i].actual_code = _write(tc[i].ptr, tc[i].pBuffer, tc[i].szBuffer);
        _T_STOP;
        if (tc[i].actual_code == tc[i].return_code) {
            printf("-- %s\n\
                  \r   PASS(%fsec)\n",
                   tc[i].name, _T_DIFF);
            continue;
        }
    }
}

void test_read() {
    printf("\n_read test:\n");
    TC tc[] = {
            {.name = "0 size buffer", .return_code = RC_ERR_INPUT, .ptr = (VA) 0, .szBuffer = 0},
            {.name = "Ptr out of range", .return_code = RC_ERR_SF, .ptr = (VA) 999, .szBuffer = 1},
    };

    char buf[] = "Hi";
    const size_t len = strlen(buf) + 1;

    init_test_env(len, buf, sizeof(buf), 1, len + 1);

    for (int i = 0; i < sizeof(tc) / sizeof(TC); i++) {
        tc[i].pBuffer = calloc(tc[i].szBuffer, 1);
        _T_START;
        tc[i].actual_code = _read(tc[i].ptr, tc[i].pBuffer, tc[i].szBuffer);
        _T_STOP;
        if ((tc[i].actual_code == RC_SUCCESS) && (strcmp(buf, tc[i].pBuffer) != 0)) {
            tc[i].actual_code = RC_ERR_U;
        }
        if (tc[i].actual_code == tc[i].return_code) {
            printf("-- %s\n\
                  \r   PASS(%fsec)\n",
                   tc[i].name, _T_DIFF);
            continue;
        }
        printf("-- %s\n\
              \r   FAIL!(%fsec)\n\
              \r   Return code is: %s\n",
               tc[i].name, _T_DIFF, code_to_str(tc[i].actual_code));
    }
}

void test_free() {
    printf("\n_free test:\n");

    TC tc[] = {
            {.name = "\"Hi\"", .ptr = (VA) 0},
            {.name = "Negative ptr", .return_code = RC_ERR_SF, .ptr = (VA) -1},
            {.name = "Ptr out of range", .return_code = RC_ERR_SF, .ptr = (VA) 999},
    };

    char buf[] = "Hi";
    const size_t len = strlen(buf) + 1;

    memory_nodel *mem = init_test_env(len, buf, sizeof(buf), 1, len + 1);

    char *rbuf = calloc(sizeof(buf), 1);
    for (int i = 0; i < sizeof(tc) / sizeof(TC); i++) {
        _T_START;
        tc[i].actual_code = _free(tc[i].ptr);
        _T_STOP;

        for (int i = 0; i < sizeof(buf); i++) {
            rbuf[i] = *((char *) (mem->physical_address + i));
        }
        if ((tc[i].actual_code == RC_SUCCESS) &&
        (strcmp(buf, rbuf) != 0)) {
            tc[i].actual_code = RC_ERR_U;
        }

        if (tc[i].actual_code == tc[i].return_code) {
            printf("-- %s\n\
                  \r   PASS(%fsec)\n",
                   tc[i].name, _T_DIFF);
            continue;
        }
        printf("-- %s\n\
              \r   FAIL!(%fsec)\n\
              \r   Return code is: %s\n",
               tc[i].name, _T_DIFF, code_to_str(tc[i].actual_code));
    }

    free(rbuf);
}

int main(int argc, char **argv) {
    test_init();
    test_malloc();
    test_write();
    test_read();
    test_free();
    return 0;
}

