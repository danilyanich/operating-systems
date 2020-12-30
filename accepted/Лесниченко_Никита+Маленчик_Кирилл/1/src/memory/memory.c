//
//  memory.c
//  memory-manager
//
//  Created by Kiril Malenchik on 10/10/20.
//

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "memory.h"

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

// элемент списка
typedef struct list_node {
    struct list_node *next;
    int size;
    m_id m_id;
    bool deleted_chunk;
} node_chunk;

// список
typedef struct list {
    int size;
    node_chunk *head;
    node_chunk *tail;
} chunks_list;

// Инициализация массива
chunks_list * create_list()
{
    chunks_list *lt = malloc(sizeof(chunks_list));

    lt->size = 0;
    lt->head = NULL;
    lt->tail = lt->head;

    return lt;
}

// Добавляем элемент в начало списка
void list_push(chunks_list *lt, m_id ptr_chunk, int size_chunk)
{
    node_chunk * node = malloc(sizeof(node_chunk));
    node->m_id = ptr_chunk;
    node->size = size_chunk;
    
    node->next = lt->head;

    lt->head = node;
    lt->size += 1;
}

// Добавляем элемент в конец списка
void list_push_back(chunks_list *lt, m_id ptr_chunk, int size_chunk, bool deleted_chunk)
{
    node_chunk * node = malloc(sizeof(node_chunk));
    node->m_id = ptr_chunk;
    node->size = size_chunk;
    node->deleted_chunk = deleted_chunk;
    
    if(lt->tail != NULL)
        lt->tail->next = node;
    else {
        lt->head = node;
    }

    lt->tail = node;
    lt->size += 1;
}

chunks_list *list_of_chunks;


m_id m_malloc(int size_of_chunk, m_err_code* error) {
    
    node_chunk *temp_node_chunk;
    temp_node_chunk = list_of_chunks->head;
    int all_size = 0;
    
    if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
      *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
      return NULL;
    }
    
    if (list_of_chunks->size == 0){
        
        _g_bytes_allocated += size_of_chunk;
        
        list_push_back(list_of_chunks, _g_allocator_memory + _g_bytes_allocated, size_of_chunk, false);

        *error = M_ERR_OK;
        return _g_allocator_memory + _g_bytes_allocated;
    }
    
    if (list_of_chunks->head->deleted_chunk == true && list_of_chunks->head->size > size_of_chunk) {
        
        list_of_chunks->head->m_id = _g_allocator_memory + all_size;
        list_of_chunks->head->deleted_chunk = false;
        
        *error = M_ERR_OK;
        return _g_allocator_memory + all_size;
        
    }
    
    if (list_of_chunks->tail->deleted_chunk == true && list_of_chunks->tail->size > size_of_chunk) {
        
        list_of_chunks->tail->m_id = _g_allocator_memory + all_size;
        list_of_chunks->tail->deleted_chunk = false;
        
        *error = M_ERR_OK;
        return _g_allocator_memory + all_size;
        
    }
    
    for (int i = 0; i < list_of_chunks->size; i++) {
        
        all_size += temp_node_chunk->size;
        
        if (temp_node_chunk->next != NULL && temp_node_chunk->next->m_id == NULL && temp_node_chunk->next->size > size_of_chunk) {
            
            all_size += temp_node_chunk->next->size;
            
            temp_node_chunk->next->m_id = _g_allocator_memory + all_size;
            temp_node_chunk->next->deleted_chunk = false;
            
            *error = M_ERR_OK;
            return _g_allocator_memory + all_size;
    
        }
        
        temp_node_chunk = temp_node_chunk->next;
        
    }

  _g_bytes_allocated += size_of_chunk;
    
    list_push_back(list_of_chunks, _g_allocator_memory + _g_bytes_allocated, size_of_chunk, false);

  *error = M_ERR_OK;
  return _g_allocator_memory + _g_bytes_allocated;
}


void m_free(m_id ptr, m_err_code* error) {
    
    if (list_of_chunks->head == NULL && list_of_chunks->tail == NULL){
        return;
    }
    
    if (list_of_chunks->head->deleted_chunk == true) {
        
        *error = M_ERR_ALREADY_DEALLOCATED;
        return;
        
    } else if (list_of_chunks->head->deleted_chunk == false){
        
        list_of_chunks->head->m_id = NULL;
        list_of_chunks->head->deleted_chunk = true;
        memset(ptr,0,list_of_chunks->head->size);
        
        *error = M_ERR_OK;
        return;
        
    }
    
    if (list_of_chunks->tail->deleted_chunk == true) {
        
        *error = M_ERR_ALREADY_DEALLOCATED;
        return;
        
    } else if (list_of_chunks->tail->deleted_chunk == false) {
        
        list_of_chunks->tail->m_id = NULL;
        list_of_chunks->tail->deleted_chunk = true;
        memset(ptr,0,list_of_chunks->tail->size);
        
        *error = M_ERR_OK;
        return;
        
    }
    
    node_chunk *temp_node_chunk;
    temp_node_chunk = list_of_chunks->head;
    
    for (int i = 0; i < list_of_chunks->size; i++) {
        
        if (temp_node_chunk->next->deleted_chunk == true){
            *error = M_ERR_ALREADY_DEALLOCATED;
            return;
        }
        
        if (temp_node_chunk->next->deleted_chunk == false && temp_node_chunk->next->m_id == ptr) {
            
            temp_node_chunk->next->m_id = NULL;
            memset(ptr,0,temp_node_chunk->next->size);
            
            *error = M_ERR_OK;
            return;
        }
        
        temp_node_chunk = temp_node_chunk->next;
        
    }
    
    
    *error = M_ERR_INVALID_CHUNK;
}


void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {
    
    node_chunk *temp_node_chunk;
    temp_node_chunk = list_of_chunks->head;
    
    for (int i = 0; i < list_of_chunks->size; i++){
        
        if (temp_node_chunk->m_id == read_from_id) {
            if (size_to_read > temp_node_chunk->size){
                *error = M_ERR_OUT_OF_BOUNDS;
                return;
            }
            
            memcpy(read_to_buffer, read_from_id, size_to_read);
            *error = M_ERR_OK;
            return;
        }
        
        temp_node_chunk = temp_node_chunk->next;
        
    }
    
        *error = M_ERR_INVALID_CHUNK;
        return;
    
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    
    node_chunk *temp_node_chunk;
    temp_node_chunk = list_of_chunks->head;
    
    for (int i = 0; i < list_of_chunks->size; i++) {
        
        if (temp_node_chunk->m_id == write_to_id) {
            if (size_to_write > temp_node_chunk->size){
                *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
                return;
            }
            
            else {
                memcpy(write_to_id, write_from_buffer, size_to_write);
                *error = M_ERR_OK;
                return;
            }
        }
        
        temp_node_chunk = temp_node_chunk->next;
    }
    

        *error = M_ERR_INVALID_CHUNK;
        return;
    
}

void m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;

    list_of_chunks = create_list();
    
}

void* getAllocatorMemory(){
    return _g_allocator_memory;
}

void deleteChunkMemory(m_id* m_id) {
    *m_id = NULL;
}
