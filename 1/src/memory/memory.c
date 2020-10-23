#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"


typedef struct chunk {
    int size;
    int start_pointer;
    struct chunk *next;
} chunk;

typedef struct cache {
    int start;
    int size;
    int count;
    struct cache* next;
} cache;

int max_cache = 300;
void* cache_memory = NULL;

typedef struct memory_manager {
    void *memory;
    int allocated_memory;
    int max_allocated;
    chunk *first_chunk;
    chunk *last_chunk;
    cache *_cache;
    int DEALLOCATED;
} memory_manager;

memory_manager my_manager = {.memory = NULL, .allocated_memory = 0, .max_allocated = 0, .first_chunk = NULL, ._cache = NULL};

bool find_free_space(cache** cur, int need_space){
    if(my_manager._cache == NULL)return false;
    for (cache* temp = my_manager._cache; temp != NULL; temp = temp->next){
        if (temp->next != NULL){
            int free_s = temp->next->start - (temp->start + temp->size);
            if(need_space < free_s){
                *cur = temp;
                return true;
            }
        }else{
            if(need_space < (max_cache - (temp->start + temp->start))){
                *cur = temp;
                return true;
            }
        }
    }
    return false;
}

cache* find_cache(int start_addr){
    for (cache* temp = my_manager._cache; temp != NULL; temp = temp->next){
        if (temp->start == start_addr){
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
    for (cache* temp = my_manager._cache; temp != NULL; temp = temp->next){
        if (temp->next != NULL && temp->next->count > max_count){
            prev = temp;
            max_count = temp->next->next->count;
        }
    }
    if (prev){
        prev->next = prev->next->next;
    } else {
        my_manager._cache = my_manager._cache->next;
    }
    return true;
}

void write_cache(chunk* to_write_chunk){
    cache* e = find_cache(to_write_chunk->start_pointer);
    if (e)return;
    if (to_write_chunk->size >= max_cache){
        return;
    }
    cache* write_to = NULL;
    bool ans = find_free_space(&write_to, to_write_chunk->size);
    while (!ans){
        delete_cache_el();
        ans = find_free_space(&write_to, to_write_chunk->size);
    }
    cache* temp = write_to->next;
    cache* new_cache;
    new_cache->start = write_to->start + write_to->size;
    new_cache->size = to_write_chunk->size;
    new_cache->count = 0;
    memcpy(cache_memory + new_cache->start, my_manager.memory + to_write_chunk->start_pointer, to_write_chunk->size);
    write_to->next = new_cache;
    new_cache->next = temp;
}

m_err_code read_cache(segment_addr addr, void* buffer, size_t read_size){
    cache* check = find_cache(*addr);
    if (check && read_size < check->size){
       memcpy(buffer, cache_memory + check->start, read_size);
       return M_ERR_OK;
    }
    return M_ERR_OUT_OF_BOUNDS;
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
        my_manager.first_chunk = NULL;
        my_manager.last_chunk = NULL;
        my_manager.allocated_memory = 0;
        free(my_manager.first_chunk);
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

    my_manager.DEALLOCATED = -1;
    my_manager.memory = malloc(size);
    cache_memory = malloc(max_cache);
    my_manager.allocated_memory = 0;
    my_manager.max_allocated = size;
    my_manager.first_chunk = NULL;
    my_manager.last_chunk = NULL;
}

