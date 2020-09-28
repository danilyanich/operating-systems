#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "memory.h"


typedef struct chunk {
    size_t size;
    size_t start_pointer;
    size_t shift;
    struct chunk *next;
} chunk;

typedef struct memory_manager {
    void *memory;
    size_t allocated_memory;
    size_t max_allocated;
    chunk *first_chunk;
    chunk *last_chunk;
} memory_manager;

typedef struct cache {
    void *data;
    size_t size;
} cache;

memory_manager my_manager = {.memory = NULL, .allocated_memory = 0, .max_allocated = 0, .first_chunk = NULL};

chunk* m_malloc(int size_of_chunk, m_err_code *error) {
    if (my_manager.allocated_memory + size_of_chunk > my_manager.max_allocated) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }
    chunk *temp = malloc(sizeof(chunk));
    temp->next = NULL;
    temp->start_pointer = my_manager.allocated_memory;
    temp->shift = 0;
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
    return temp;
}


m_err_code m_free(chunk *ptr) {
    if(ptr == NULL)return M_ERR_ALREADY_DEALLOCATED;
    if (ptr->start_pointer == my_manager.first_chunk->start_pointer && my_manager.first_chunk->next == NULL){
        my_manager.first_chunk = NULL;
        my_manager.last_chunk = NULL;
        my_manager.allocated_memory = 0;
        free(ptr);
        return M_ERR_OK;
    }
    bool is_find = false;
    size_t destination;
    for(chunk* start = my_manager.first_chunk; start != NULL; start = start->next){
        if (ptr->start_pointer == start->start_pointer){
            is_find = true;
            destination = ptr->start_pointer;
            start = start->next;
            my_manager.first_chunk = start;
        }
        if (is_find){
            memcpy(my_manager.memory+destination, my_manager.memory + start->start_pointer, start->size);
            start->start_pointer = destination;
            destination = start->start_pointer + start->size;
        }
        else if (start->next != NULL && ptr->start_pointer == start->next->start_pointer){
            is_find = true;
            destination = ptr->start_pointer;
            if (start->next->next != NULL){
                start->next = start->next->next;
            }else{
                my_manager.last_chunk = start;
                start->next = NULL;
            }
        }
    }
    if (is_find){
        my_manager.allocated_memory -= ptr->size;
        free(ptr);
        return M_ERR_OK;
    }
    return M_ERR_INVALID_CHUNK;
}


m_err_code m_read(chunk* read_from_id, void *read_to_buffer, int size_to_read) {
    if(read_from_id == NULL || read_from_id->start_pointer > my_manager.max_allocated)return M_ERR_INVALID_CHUNK;
    if(size_to_read > read_from_id->size)return M_ERR_OUT_OF_BOUNDS;
    memcpy(read_to_buffer, my_manager.memory + read_from_id->start_pointer, size_to_read);
    return M_ERR_OK;
}


m_err_code m_write(chunk* write_to_id, void *write_from_buffer, int size_to_write) {
    if(write_to_id == NULL || write_to_id->start_pointer > my_manager.max_allocated)return M_ERR_INVALID_CHUNK;
    if(write_to_id->size - write_to_id->shift < size_to_write)return M_ERR_OUT_OF_BOUNDS;
    memcpy(my_manager.memory + (write_to_id->start_pointer + write_to_id->shift), write_from_buffer, size_to_write);
    write_to_id->shift += size_to_write;
    return M_ERR_OK;
}


void m_init(int size) {
    if (my_manager.memory != NULL) free(my_manager.memory);

    my_manager.memory = malloc(size);
    my_manager.allocated_memory = 0;
    my_manager.max_allocated = size;
    my_manager.first_chunk = NULL;
    my_manager.last_chunk = NULL;
}
