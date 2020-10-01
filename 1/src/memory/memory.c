#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"

#define CACHE_SIZE 5

typedef struct chunk {
    size_t size;
    size_t start_pointer;
    struct chunk *next;
} chunk;

typedef struct cache {
    size_t free_chunk;
    chunk* segments[CACHE_SIZE];
} cache;

typedef struct memory_manager {
    void *memory;
    size_t allocated_memory;
    size_t max_allocated;
    chunk *first_chunk;
    chunk *last_chunk;
    cache _cache;
} memory_manager;

memory_manager my_manager = {.memory = NULL, .allocated_memory = 0, .max_allocated = 0, .first_chunk = NULL};

m_err_code read_cache(segment_addr addr, void* buffer, size_t read_size){
    for (int i = 0; i < CACHE_SIZE; ++i) {
        if (my_manager._cache.segments[i] != NULL && my_manager._cache.segments[i]->start_pointer == *addr){
            chunk* selected_chunk = my_manager._cache.segments[i];
            if(read_size > selected_chunk->size)return M_ERR_OUT_OF_BOUNDS;
            memcpy(buffer, my_manager.memory + selected_chunk->start_pointer, read_size);
            return M_ERR_OK;
        }
    }
    return M_WARN_NO_IN_CACHE;
}

void write_cache(chunk* to_write_chunk){
    my_manager._cache.segments[my_manager._cache.free_chunk] = to_write_chunk;
    my_manager._cache.free_chunk++;
    if(my_manager._cache.free_chunk >= CACHE_SIZE)my_manager._cache.free_chunk -= CACHE_SIZE;
}

segment_addr m_malloc(int size_of_chunk, m_err_code *error) {
    if (my_manager.allocated_memory + size_of_chunk > my_manager.max_allocated) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return &my_manager.max_allocated + 1;
    }
    chunk *temp = malloc(sizeof(chunk));
    temp->next = NULL;
    temp->start_pointer = my_manager.allocated_memory;
    temp->size = size_of_chunk;
    if (my_manager.first_chunk == NULL) {
        my_manager.first_chunk = temp;
        my_manager.last_chunk = temp;
    } else {
        my_manager.last_chunk->next = temp;
        my_manager.last_chunk = temp;
    }
    my_manager.allocated_memory += size_of_chunk;

    *error = M_ERR_OK;
    return &temp->start_pointer;
}


m_err_code m_free(segment_addr ptr) {
    if(*ptr > my_manager.max_allocated)return M_ERR_ALREADY_DEALLOCATED;
    if (*ptr == my_manager.first_chunk->start_pointer && my_manager.first_chunk->next == NULL){
        my_manager.first_chunk = NULL;
        my_manager.last_chunk = NULL;
        my_manager.allocated_memory = 0;
        free(my_manager.first_chunk);
        return M_ERR_OK;
    }
    chunk* to_delete = NULL;
    bool is_find = false;
    size_t destination;
    for(chunk* start = my_manager.first_chunk; start != NULL; start = start->next){
        if (*ptr == start->start_pointer){
            to_delete = start;
            is_find = true;
            destination = *ptr;
            start = start->next;
            my_manager.first_chunk = start;
        }
        if (is_find){
            memcpy(my_manager.memory+destination, my_manager.memory + start->start_pointer, start->size);
            start->start_pointer = destination;
            destination = start->start_pointer + start->size;
        }
        else if (start->next != NULL && *ptr == start->next->start_pointer){
            is_find = true;
            to_delete = start->next;
            destination = *ptr;
            if (start->next->next != NULL){
                start->next = start->next->next;
            }else{
                my_manager.last_chunk = start;
                start->next = NULL;
            }
        }
    }
    if (is_find){
        my_manager.allocated_memory -= to_delete->size;
        free(to_delete);
        return M_ERR_OK;
    }
    return M_ERR_INVALID_CHUNK;
}


m_err_code m_read(segment_addr read_from_id, void *read_to_buffer, int size_to_read) {
    int error = read_cache(read_from_id, read_to_buffer, size_to_read);
    if(error == M_ERR_OK)return M_ERR_OK;
    if(error == M_ERR_OUT_OF_BOUNDS)return M_ERR_OUT_OF_BOUNDS;

    chunk* temp = NULL;
    for(chunk* i = my_manager.first_chunk; i != NULL; i = i->next){
        if(i->start_pointer == *read_from_id){
            temp = i;
            write_cache(temp);
        }
    }
    if(temp == NULL)return M_ERR_INVALID_CHUNK;
    if(size_to_read > temp->size)return M_ERR_OUT_OF_BOUNDS;
    memcpy(read_to_buffer, my_manager.memory + temp->start_pointer, size_to_read);
    return M_ERR_OK;
}


m_err_code m_write(segment_addr write_to_id, void *write_from_buffer, int size_to_write) {
    chunk* temp = NULL;
    for(chunk* i = my_manager.first_chunk; i != NULL; i = i->next){
        if(i->start_pointer == *write_to_id){
            temp = i;
        }
    }
    if(temp == NULL)return M_ERR_INVALID_CHUNK;
    if(temp->size < size_to_write)return M_ERR_OUT_OF_BOUNDS;
    memcpy(my_manager.memory + temp->start_pointer, write_from_buffer, size_to_write);
    return M_ERR_OK;
}


void m_init(int size) {
    if (my_manager.memory != NULL) free(my_manager.memory);

    my_manager._cache.free_chunk = 0;
    for (int i = 0; i < CACHE_SIZE; ++i) {
        my_manager._cache.segments[i] = NULL;
    }
    my_manager.memory = malloc(size);
    my_manager.allocated_memory = 0;
    my_manager.max_allocated = size;
    my_manager.first_chunk = NULL;
    my_manager.last_chunk = NULL;
}

