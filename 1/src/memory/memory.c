#include <stdlib.h>
#include <string.h>

#include "memory.h"


char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

m_id find_free_segment(int);
void defragmentation();

struct virtual_memory memory;


//-------------------------------------------------------------------------------------------------
m_id m_malloc(
    int size_of_chunk, 
    m_err_code* error
){
    m_id segment = find_free_segment(size_of_chunk);
    
    if (segment == NULL){
        //defragmentation();
        segment = find_free_segment(size_of_chunk);
    } 

    if (segment == NULL){
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    return segment;
}


//-------------------------------------------------------------------------------------------------
/*This function allows to find free segment*/
m_id find_free_segment(
    int size_of_chunk
){
    int size = size_of_chunk;
    for (int i = 0; i < memory.number_of_pages; i++){
        bool is_oversize = false;

        m_id current = (memory.pages + i) -> begin;

        while(current->is_used || current->size < size){
            current = current -> next;
            if (current >=  ((memory.pages + i)->begin
                            + (memory.pages + i)->size)) {
                is_oversize = true;
                break;
            }
            if (current -> not_calling > memory.temporary_locality){
                current->is_used = false;
                if (current->size >= size){
                    break;
                }
            }
        }

        if (!is_oversize) {
            if (current->next == NULL){
                m_id next = current + 1;
                next->is_used = false;
                next -> next = NULL;
                next -> size = current -> size - size;
                next -> data = current -> data + size;
                current -> next = next;
            }else{
                if (current -> size != size){
                    m_id next = malloc(sizeof(m_id));
                    next->is_used = false;
                    next->size = current->size - size;
                    next->data = current->data + size;
                    next->next = current->next;
                    current -> next = next;
                }
            }
            current -> size = size;
            current -> not_calling = 0;
            current -> is_used = true;
            return current;
        }
    }
    return NULL;
}


//-------------------------------------------------------------------------------------------------
void m_free(
    m_id ptr, 
    m_err_code* error
){
    ptr -> is_used = false;
    *error = M_ERR_OK;
}


//-------------------------------------------------------------------------------------------------
void m_read(
    m_id read_from_id,
    void* read_to_buffer, 
    int size_to_read, 
    m_err_code* error
){
    read_from_id -> not_calling = 0;

    memcpy(read_to_buffer, read_from_id->data, size_to_read);

    *error = M_ERR_OK;
    for (int i = 0; i < memory.number_of_pages; i++){
        m_id current = (memory.pages+i) -> begin;
        while(current != NULL){
            if (current != read_from_id){
                current -> not_calling++;
            }
            current = current -> next;
        }
    }
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
  memcpy(write_to_id, write_from_buffer, size_to_write);
  *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;
}
