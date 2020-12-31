//
// Created by diane on 11/19/20.
//

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mmemory.h"


typedef struct chunk {
    int size;
    int start_pointer;
    struct chunk *next;
} chunk;

typedef struct cache {
    int start_pointer;
    int size;
    int count;
    int chunk_addr;
    struct cache* next_chunk;
} cache;

int max_cache = 300;
void* cache_memory = NULL;

typedef struct memory_manager {
    void *memory_m;
    int allocated_memory;
    int max_allocated;
    chunk *first_chunk;
    chunk *last_chunk;
    cache *_cache;
    int DEALLOCATED;
} memory_manager;

memory_manager my_manager = {.memory_m = NULL, .allocated_memory = 0, .max_allocated = 0, .first_chunk = NULL, ._cache = NULL};

bool find_free_space(cache** current, int space_needed){
    if(my_manager._cache == NULL)return false;
    for (cache* temp = my_manager._cache; temp != NULL; temp = temp->next_chunk){
        if (temp->next_chunk != NULL){
            int free_s = temp->next_chunk->start_pointer - (temp->start_pointer + temp->size);
            if(space_needed < free_s){
                *current = temp;
                return true;
            }
        }else{
            if(space_needed < (max_cache - (temp->start_pointer + temp->start_pointer))){
                *current = temp;
                return true;
            }
        }
    }
    return false;
}

cache* find_cache(int start_pointer){
    for (cache* temp = my_manager._cache; temp != NULL; temp = temp->next_chunk){
        if (temp->chunk_addr == start_pointer){
            temp->count++;
            return temp;
        }
    }
    return NULL;
}

bool delete_cache_el(){
    int max_count = -1;
    cache* prev = NULL;
    if (my_manager._cache != NULL){
        max_count = my_manager._cache->count;
    } else return false;
    for (cache* temp = my_manager._cache; temp != NULL; temp = temp->next_chunk){
        if (temp->next_chunk != NULL && temp->next_chunk->count > max_count){
            prev = temp;
            max_count = temp->next_chunk->next_chunk->count;
        }
    }
    if (prev){
        cache* temp = prev->next_chunk;
        prev->next_chunk = prev->next_chunk->next_chunk;
        free(temp);
    } else {
        cache* temp = my_manager._cache;
        my_manager._cache = my_manager._cache->next_chunk;
        free(temp);
    }
    return true;
}

void delete_cache(int chunk_pointer)
{
    cache* prev = NULL;
    if (my_manager._cache == NULL) return;

    for (cache* temp = my_manager._cache; temp != NULL; temp = temp->next_chunk){
        if (temp->chunk_addr == chunk_pointer)
        {
            if (prev != NULL)
            {
                prev->next_chunk = temp->next_chunk;
                free(temp);
                return;
            } else
            {
                my_manager._cache = temp->next_chunk;
                free(temp);
                return;
            }
        }
    }
}

void write_cache(chunk* to_chunk){
    cache* e = find_cache(to_chunk->start_pointer);
    if (e)return;
    if (to_chunk->size >= max_cache){
        return;
    }
    cache* write_to = NULL;
    bool ans = find_free_space(&write_to, to_chunk->size);
    bool del = true;
    while (!ans && del){
        del = delete_cache_el();
        ans = find_free_space(&write_to, to_chunk->size);
    }
    if (!del){
        cache* temp = malloc(sizeof(struct cache));
        temp->start_pointer = 0;
        temp->count = 0;
        temp->chunk_addr = to_chunk->start_pointer;
        temp->size = to_chunk->size;
        temp->next_chunk = NULL;
        my_manager._cache = temp;
        memcpy(cache_memory, my_manager.memory_m + to_chunk->start_pointer, to_chunk->size);
    } else {
        cache* temp = write_to->next_chunk;
        cache* new_cache = malloc(sizeof(struct cache));
        new_cache->start_pointer = write_to->start_pointer + write_to->size;
        new_cache->size = to_chunk->size;
        new_cache->count = 0;
        new_cache->chunk_addr = to_chunk->start_pointer;
        memcpy(cache_memory + new_cache->start_pointer, my_manager.memory_m + to_chunk->start_pointer, to_chunk->size);
        write_to->next_chunk = new_cache;
        new_cache->next_chunk = temp;
    }
}

m_err_code read_cache(segment_addr addr, void* buffer, size_t read_size){
    cache* check = find_cache(*addr);
    if (check && read_size < check->size){
        memcpy(buffer, cache_memory + check->start_pointer, read_size);
        return M_ERR_OK;
    }
    return M_ERR_OUT_OF_BOUNDS;
}

void over_write_cache(int chunk_addr, chunk* ch)
{
    cache* temp = find_cache(chunk_addr);
    if (temp)
    {
        memcpy(cache_memory + temp->start_pointer, my_manager.memory_m + ch->start_pointer, ch->size);
    }
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
    if (*ptr == my_manager.DEALLOCATED)return M_ERR_ALREADY_DEALLOCATED;
    if(*ptr > my_manager.max_allocated)return M_ERR_INVALID_CHUNK;
    if (*ptr == my_manager.first_chunk->start_pointer && my_manager.first_chunk->next == NULL){
        delete_cache(my_manager.first_chunk->start_pointer);
        free(my_manager.first_chunk);
        my_manager.first_chunk = NULL;
        my_manager.last_chunk = NULL;
        my_manager.allocated_memory = 0;
        *ptr = my_manager.DEALLOCATED;
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
            memcpy(my_manager.memory_m + destination, my_manager.memory_m + start->start_pointer, start->size);
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
        delete_cache(to_delete->start_pointer);
        free(to_delete);
        *ptr = my_manager.DEALLOCATED;
        return M_ERR_OK;
    }
    return M_ERR_ALREADY_DEALLOCATED;
}


m_err_code m_read(segment_addr read_from_id, void *read_to_buffer, int size_to_read) {
    int error = read_cache(read_from_id, read_to_buffer, size_to_read);
    if(error == M_ERR_OK)return M_ERR_OK;

    chunk* temp = NULL;
    for(chunk* i = my_manager.first_chunk; i != NULL; i = i->next){
        if(i->start_pointer == *read_from_id){
            temp = i;
            write_cache(temp);
        }
    }
    if(temp == NULL)return M_ERR_INVALID_CHUNK;
    if(size_to_read > temp->size)return M_ERR_OUT_OF_BOUNDS;
    memcpy(read_to_buffer, my_manager.memory_m + temp->start_pointer, size_to_read);
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
    over_write_cache(*write_to_id, temp);
    memcpy(my_manager.memory_m + temp->start_pointer, write_from_buffer, size_to_write);
    return M_ERR_OK;
}


void m_init(int size) {
    if (my_manager.memory_m != NULL) free(my_manager.memory_m);

    my_manager.DEALLOCATED = -1;
    my_manager.memory_m = malloc(size);
    cache_memory = malloc(max_cache);
    my_manager.allocated_memory = 0;
    my_manager.max_allocated = size;
    my_manager.first_chunk = NULL;
    my_manager.last_chunk = NULL;
}

