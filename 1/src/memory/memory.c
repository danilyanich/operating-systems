#include <stdlib.h>
#include <string.h>

#include "memory.h"


char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

struct block* top = NULL;
struct block* begin = NULL;

m_id m_malloc(int size_of_chunk, m_err_code* error) {

    if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    struct block* tmp = malloc(sizeof(*tmp));
    struct block* current_block = begin;
    int result = 0;

    if (begin == NULL){
        begin = tmp;
        tmp->memory = _g_allocator_memory + size_of_chunk;
        
    }
    else{
         result= insertBlock(tmp, current_block, size_of_chunk); 
    }
   
    if (result) {
        printf("\n%d, %d ", (int)_g_allocator_memory, size_of_chunk);
        printf("\n%d ", (int)tmp->memory);
        *error = M_ERR_OK;
        return (m_id)tmp;
    }
    else {
        tmp->size = size_of_chunk;
        tmp->prev = top;
        tmp->next = NULL;
        tmp->isNextNull = 1;
        if (tmp->prev != NULL) {
            tmp->memory = (char*)top->memory + size_of_chunk;
            tmp->prev->next = tmp;
            tmp->prev->isNextNull = 0;
        }
        top = tmp;
        printf("\n%d, %d ", (int)_g_allocator_memory, size_of_chunk);
        printf("\n%d ", (int)tmp->memory);
      
     }
 
  *error = M_ERR_OK;
  return (m_id)tmp;

}

int insertBlock(struct block* tmp, struct block* current_block, int size_of_chunk) {


    if ((char*)begin->memory - _g_allocator_memory > size_of_chunk) {

        tmp->memory = _g_allocator_memory + size_of_chunk;
        tmp->size = size_of_chunk;
        tmp->isNextNull = 1;
        tmp->next = begin;
        tmp->prev = NULL;

        begin->prev = tmp;
        begin = tmp;
        return 1;
    
    }
    else if ((char*)begin->memory - _g_allocator_memory == size_of_chunk) {
   
        tmp->memory = _g_allocator_memory + size_of_chunk;
        tmp->size = size_of_chunk;
        tmp->isNextNull = 0;
        tmp->next = begin;

        tmp->prev = NULL;

        begin->prev = tmp;
        begin = tmp;
        return 1;
    }
   
    while (current_block != top) {
        
        if (current_block->isNextNull == 1 &&  current_block->next != NULL)
        {
            if ((int)current_block->next->memory - (int)current_block->memory - current_block->next->size  > size_of_chunk)
            {

                tmp->memory = (char*)current_block->memory + size_of_chunk;
                tmp->size = size_of_chunk;
                tmp->isNextNull = 1;
                tmp->next = current_block->next;
                tmp->prev = current_block;

                current_block->isNextNull = 0;
                current_block->next->prev = tmp;
                current_block->next = tmp;
                return 1;
            }
            else if ((int)current_block->next->memory - (int)current_block->memory - current_block->next->size == size_of_chunk)
            {
                tmp->memory = (char*)current_block->memory + size_of_chunk;
                tmp->size = size_of_chunk;
                current_block->isNextNull = 0;
                tmp->isNextNull = 0;
                tmp->next = current_block->next;
                tmp->prev = current_block;

                current_block->next->prev = tmp;
                current_block->next = tmp;
                return 1;
            }
        }
        current_block = current_block->next;

    }
    return 0;
}
void m_free(m_id ptr, m_err_code* error) {

    struct block* ptr_new = ptr;
   

    if (ptr == NULL) {
        *error = M_ERR_ALREADY_DEALLOCATED;
        return;
    }

    struct block* tmp;
    if (ptr_new->memory == top->memory)
    {
        tmp = top;
        top = ptr_new->prev;
        ptr_new->prev->isNextNull = 1;
        free(tmp);

    }
    else if(ptr_new->memory == begin->memory)
    {
        tmp = begin;
        begin = ptr_new->next;
        free(tmp);
    }
        
    else
    {
        tmp = ptr_new;
        ptr_new->prev->next = tmp->next;
        ptr_new->next->prev = tmp->prev;
        ptr_new->prev->isNextNull = 1;
        free(ptr_new);
    }
    *error = M_ERR_OK;
}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
    struct block* ptr_new = read_from_id;

    if (size_to_read > ptr_new->size) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    if (&read_from_id != NULL && ptr_new->memory != NULL)
    {
        memcpy(read_to_buffer, ptr_new->memory, size_to_read);
    }
    else {
        *error = M_ERR_INVALID_CHUNK;
        return;
        }
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    struct block* ptr_new = write_to_id;

    if (write_to_id == NULL) {
        *error = M_ERR_INVALID_CHUNK;
        return;
    }
   
    if (size_to_write > ptr_new->size) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    memcpy(ptr_new->memory, write_from_buffer, size_to_write);
     *error = M_ERR_OK;
}


void  m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;

  
}
