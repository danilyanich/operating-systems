#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;
struct mem_chunk* chunks_list[50];//array of allocated chunks
struct mem_chunk* mem_chunks;
//struct mem_chunk* deleted_chunks[50];//array of deallocated chunks
int size=0;//size of chunks_list array
//int deleted_size=0;//size of deleted_chunks array

void debug(){
        printf("%s\n", _g_allocator_memory+18+28);
}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
  if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
    return NULL;
  }
  struct mem_chunk* tmp=malloc(sizeof(struct mem_chunk));
  if(mem_chunks==NULL){//initializing first element of list
      tmp->ptr=_g_allocator_memory+_g_bytes_allocated;
      tmp->size=size_of_chunk;
      tmp->next=NULL;
      mem_chunks = tmp;
  } else{//when add new element to list
      struct mem_chunk* prev=mem_chunks;//variables for iterating through the list
      struct mem_chunk* next=mem_chunks->next;//
      while (1){
          if(next==NULL){//when the next element is not exist
              tmp->ptr=_g_allocator_memory+_g_bytes_allocated;//allocation
              tmp->size=size_of_chunk;//
              tmp->next=NULL;//adding to the list
              prev->next=tmp;//
              tmp->prev = prev;//
              break;
          } else{
              prev=prev->next;
              next=prev->next;
          }
      }

  }
  _g_bytes_allocated += size_of_chunk;//increase shift for next allocation
  *error = M_ERR_OK;
  return (m_id)tmp;
}


void m_free(m_id ptr, m_err_code* error) {
    struct mem_chunk** tmp=ptr;
    if(*tmp==NULL){//checking for previous allocation
        printf("%s","DEALLOCATED ALREADY");
        *error=M_ERR_ALREADY_DEALLOCATED;
        return;
    }

    int begin_of_chunk=0;//variable for shift
    _g_bytes_allocated -= (*tmp)->size;//decrease size of allocated bytes to prevent gap in memory after new allocation
    if(*tmp==mem_chunks){
        mem_chunks = mem_chunks->next;
        free(*tmp);
        *tmp = NULL;
    } else{
        struct mem_chunk*next=(*tmp)->next;//deleting from list
        struct mem_chunk*prev=(*tmp)->prev;//
        prev->next=next;//
        next->prev = prev;//
        free(*tmp);
        *tmp = NULL;
    }
    struct mem_chunk*it=mem_chunks;
    while (it!=NULL){//rewriting of the array exclude deallocated chunk
        memcpy(_g_allocator_memory + begin_of_chunk, it->ptr, it->size);
        it->ptr = _g_allocator_memory + begin_of_chunk;
        begin_of_chunk += it->size;
        it = it->next;
    }


}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
    struct mem_chunk* it=mem_chunks;//iterator from the beginning of list
    if(read_from_id==NULL){//checking for deallocation
        *error=M_ERR_INVALID_CHUNK;
        printf("%s\n", "invalid chunk");
        return;
    } else while (1){//checking for existing
        if(it==NULL){
            *error=M_ERR_INVALID_CHUNK;
            printf("%s\n", "invalid chunk");
            return;
        } else if(it==read_from_id)
            break;
            it = it->next;
    }
    if(it->size>size_to_read){//checking for size
        *error=M_ERR_OUT_OF_BOUNDS;
        printf("%s\n", "out of bounds");
        return;
    }
    memcpy(read_to_buffer, it->ptr, size_to_read);
    *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    struct mem_chunk* it=mem_chunks;//iterator from the beginning of list
    if(write_to_id==NULL){//checking for deallocation
        *error=M_ERR_INVALID_CHUNK;
        printf("%s\n", "invalid chunk");
        return;
    }else while (1){//checking for existing
            if(it==NULL){
                *error=M_ERR_INVALID_CHUNK;
                printf("%s\n", "invalid chunk");
                return;
            } else if(it==write_to_id)
                break;
            it = it->next;
        }
    if(it->size<size_to_write){//checking for size
        printf("%s\n", "out of bounds");
        *error=M_ERR_OUT_OF_BOUNDS;
        return;
    }
    memcpy(it->ptr, write_from_buffer, size_to_write);
    *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);
  while(mem_chunks!=NULL){
	struct mem_chunk*tmp=mem_chunks;
	mem_chunks=mem_chunks->next;
	free(tmp);
  }
  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;


}
