//
// Created by Petri on 9/13/2020.
//

#include "../header/mmemory.h"
#include "../tests/unit_tests.h"
#include <stdlib.h>
#include <assert.h>
#include "../header/memory_model.h"

#define segment_end(s) (s->va + s->segment_length)
static memory_nodel *memoryNodel;
static segment_queue *cached_record;

void check_cache(segment_queue **s) {
    if (cached_record == *s) {
        *s = cached_record;
    }
}

void free_all() {
    if (memoryNodel == NULL) {
        return;
    }

    segment_queue *cur = memoryNodel->first_segment;
    segment_queue *prev;
    while (cur->next != NULL) {
        prev = cur;
        cur = cur->next;

        free(prev);
        prev = NULL;
    }

    if (cur == memoryNodel->first_segment) {
        memoryNodel->first_segment = NULL;
    }

    free(cur);

    free(memoryNodel);
    memoryNodel = NULL;
    cached_record = NULL;
}

void init_test_memory(memory_nodel *new_mem) {
    free_all();
    memoryNodel = new_mem;
}

segment_queue *find_space(size_t s_len) {
    assert(memoryNodel->first_segment != NULL);

    if (memoryNodel->first_segment->next == NULL) {
        return memoryNodel->first_segment;
    }

    segment_queue *s = memoryNodel->first_segment;

    while (s->next != NULL) {
        if (s->next->va - segment_end(s) >= s_len) {
            return s;
        }

        s = s->next;
    }

    if (memoryNodel->memory_size - (size_t) s->va >= s_len) {
        return s;
    }

    return NULL;
}

int _malloc(VA *ptr, size_t szBlock) {
    if ((szBlock < 1) || (szBlock > memoryNodel->memory_size)) {
        return RC_ERR_INPUT;
    }

    segment_queue *s = find_space(szBlock);
    if (s == NULL) {
        return RC_ERR_U;
    }

    if ((s == memoryNodel->first_segment) && (s->segment_length == 0)) {
        s->segment_length = szBlock;

        *ptr = s->va;

        return RC_SUCCESS;
    }

    segment_queue *next = s->next;
    s->next = calloc(1, sizeof(segment_queue));
    s->next->va = segment_end(s) + 1;
    s->next->segment_length = szBlock;
    s->next->next = next;

    *ptr = s->va;

    return RC_SUCCESS;
}

int virtual_to_segment(const VA ptr, segment_queue **segment) {
    if ((ptr < 0) || (ptr > (VA) memoryNodel->memory_size)) {
        return RC_ERR_SF;
    }

    segment_queue *temp_s = memoryNodel->first_segment;
    while (temp_s->next != NULL) {
        if (ptr <= segment_end(temp_s)) {
            *segment = temp_s;
            return RC_SUCCESS;
        }

        temp_s = temp_s->next;
    }

    return RC_ERR_U;
}

size_t get_segment_physical_address(segment_queue *s) {
    assert(s != NULL);
    return memoryNodel->physical_address + (size_t) s->va;
}

int _free(VA ptr) {
    segment_queue *segment;
    int code = virtual_to_segment(ptr, &segment);
    if (code != RC_SUCCESS) {
        return code;
    }

    if (segment == memoryNodel->first_segment) {
        memoryNodel->first_segment = segment->next;
        free(segment);
        return RC_SUCCESS;
    }

    segment_queue *prev = memoryNodel->first_segment;
    while (prev->next != segment) {
        prev = prev->next;
    }
    prev->next = segment->next;
    free(segment);
    return RC_SUCCESS;
}

void get_cache_for(segment_queue* segment) {
    if (segment != NULL && segment->next != NULL) {
        cached_record = segment->next;
    }
}

int _read(VA ptr, void *pBuffer, size_t szBuffer) {
    if ((pBuffer == NULL) || (szBuffer < 1)) {
        return RC_ERR_INPUT;
    }
    segment_queue *segment;
    int code = virtual_to_segment(ptr, &segment);
    if (code != RC_SUCCESS) {
        return code;
    }
    if (szBuffer > segment->segment_length) {
        return RC_ERR_SF;
    }
    VA bufEl;
    VA sEl;
    check_cache(&segment);
    VA pa = (VA) get_segment_physical_address(segment);
    for (int i = 0; i < szBuffer; i++) {
        bufEl = ((VA) pBuffer + i);
        sEl = (pa + i);
        assert(sEl != NULL);
        assert(bufEl != NULL);
        *bufEl = *sEl;
    }
    get_cache_for(segment);
    return RC_SUCCESS;
}

int _write(VA ptr, void *pBuffer, size_t szBuffer) {
    if ((pBuffer == NULL) || (szBuffer < 1)) {
        return RC_ERR_INPUT;
    }
    segment_queue *segment;
    int code = virtual_to_segment(ptr, &segment);
    if (code != RC_SUCCESS) {
        return code;
    }

    if (szBuffer > segment->segment_length) {
        return RC_ERR_SF;
    }
    VA bufEl;
    VA sEl;
    check_cache(&segment);
    VA pa = (VA) get_segment_physical_address(segment);
    for (int i = 0; i < szBuffer; i++) {
        bufEl = ((VA) pBuffer + i);
        sEl = (pa + i);

        assert(sEl != NULL);
        assert(bufEl != NULL);

        *sEl = *bufEl;
    }

    return RC_SUCCESS;
}

int _init(int n, int szPage) {
    if ((n < 1) || (szPage < 1)) {
        return RC_ERR_INPUT;
    }

    free_all();
    memoryNodel = calloc(1, sizeof(memory_nodel));
    memoryNodel->memory_size = n * szPage;
    memoryNodel->physical_address = (size_t) calloc(memoryNodel->memory_size, 1);
    memoryNodel->first_segment = calloc(1, sizeof(segment_queue));

    return RC_SUCCESS;
}