#include <stdlib.h>
#include <string.h>
#include "uses_cache.c"
#include "memory.h"
#include <stdio.h>



char *_g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;

static memory_node *memoryNode;
static segment *cache;

segment* check_cache(m_id ptr){
    if(cache == NULL){
        return NULL;
    }
    if(cache->va+cache->size_of_segment == ptr){
        return cache;
    } else {
        return NULL;
    }
}

void up_date_cache(){
    segment *tmp = memoryNode->first_segment;
    cache = tmp;
    while (tmp != NULL) {
        if(tmp->amount_of_uses >= cache->amount_of_uses){
            cache = tmp;
        }
        tmp = tmp->next;
    }
}

m_id find_space(int size_of_chunk, m_err_code *error) {
    if (memoryNode->first_segment == NULL) {
        segment *first = malloc(sizeof(segment));

        first->size_of_segment = size_of_chunk;
        first->va = memoryNode->physical_address;
        first->next = NULL;
        first->amount_of_uses = 0;

        cache = first;

        memoryNode->first_segment = first;
        *error = M_ERR_OK;
        return first->va + size_of_chunk;
    }

    segment *tmp = memoryNode->first_segment;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    if (memoryNode->physical_address + memoryNode->size_of_segment
        - tmp->va + tmp->size_of_segment < size_of_chunk) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    segment *end = malloc(sizeof(segment));
    end->size_of_segment = size_of_chunk;
    end->va = tmp->va + size_of_chunk + tmp->size_of_segment;
    end->next = NULL;
    end->amount_of_uses = 0;

    tmp->next = end;
    *error = M_ERR_OK;
    return end->va + end->size_of_segment;
}

m_id m_malloc(int size_of_chunk, m_err_code *error) {
    if (memoryNode->size_of_segment < size_of_chunk) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    m_id address = find_space(size_of_chunk, error);
    if (*error != M_ERR_OK) {
        return NULL;
    } else {
        return address;
    }
}

segment *find_segment(m_id ptr, m_err_code *error) {
    segment *tmp = memoryNode->first_segment;

    while (tmp != NULL) {
        if (tmp->va + tmp->size_of_segment == ptr) {
            *error = M_ERR_OK;
            return tmp;
        }

        tmp = tmp->next;
    }
    error = M_ERR_INVALID_CHUNK;
}

void m_free(m_id ptr, m_err_code *error) {
    segment *foundSeg = find_segment(ptr, error);

    if (*error != M_ERR_OK) {
        return;
    }
    if (foundSeg == memoryNode->first_segment) {
        memoryNode->first_segment = foundSeg->next;
        free(foundSeg);
        return;
    }

    segment *prev = memoryNode->first_segment;

    while (prev->next != foundSeg) {
        prev = prev->next;
    }
    prev->next = foundSeg->next;
    free(foundSeg);

    up_date_cache();

    *error = M_ERR_OK;
}

void m_read(m_id read_from_id, void *read_to_buffer, int size_to_read, m_err_code *error) {
    segment *foundSeg = check_cache(read_from_id);
    if(foundSeg == NULL){
       foundSeg = find_segment(read_from_id, error);
       if (*error != M_ERR_OK) {
            return;
        }
    }

    if (foundSeg->size_of_segment < size_to_read) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }

    foundSeg->amount_of_uses++;
    up_date_cache();

    memcpy(read_to_buffer, read_from_id, size_to_read);
    *error = M_ERR_OK;
}

void m_write(m_id write_to_id, void *write_from_buffer, int size_to_write, m_err_code *error) {
    segment *foundSeg = check_cache(write_to_id);
    if(foundSeg == NULL){
        foundSeg = find_segment(write_to_id, error);
        if (*error != M_ERR_OK) {
            return;
        }
    }

    if (foundSeg->size_of_segment < size_to_write) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }

    foundSeg->amount_of_uses++;
    up_date_cache();

    memcpy(write_to_id, write_from_buffer, size_to_write);
    *error = M_ERR_OK;
}

void m_init(int number_of_pages, int size_of_page) {
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);

    _g_allocator_memory_size = number_of_pages * size_of_page;
    //_g_allocator_memory = malloc(_g_allocator_memory_size);
    //_g_bytes_allocated = 0;

    memoryNode = malloc(sizeof(memory_node));
    memoryNode->physical_address = malloc(_g_allocator_memory_size);
    memoryNode->first_segment = NULL;
    memoryNode->size_of_segment = _g_allocator_memory_size;

    cache = malloc(sizeof(segment));
}
