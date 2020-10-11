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
        /*printf("%s\n",_g_allocator_memory);
        printf("%s\n",_g_allocator_memory+chunks_list[0]->size);
        printf("%s\n",_g_allocator_memory+chunks_list[0]->size+chunks_list[1]->size);*/
        printf("%s\n", _g_allocator_memory+13+28+17+14);

}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
  if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
    return NULL;
  }
  struct mem_chunk* tmp=malloc(sizeof(struct mem_chunk));
  if(mem_chunks==NULL){
      tmp->ptr=_g_allocator_memory+_g_bytes_allocated;
      tmp->size=size_of_chunk;
      tmp->next=NULL;
      mem_chunks = tmp;
  } else{
      struct mem_chunk* prev=mem_chunks;
      struct mem_chunk* next=mem_chunks->next;
      while (1){
          if(next==NULL){
              //printf("%d",1);
              tmp->ptr=_g_allocator_memory+_g_bytes_allocated;
              tmp->size=size_of_chunk;
              tmp->next=NULL;
              prev->next=tmp;
              tmp->prev = prev;
              break;
          } else{
              prev=prev->next;
              next=prev->next;
          }
      }

  }
  _g_bytes_allocated += size_of_chunk;
  *error = M_ERR_OK;
  return (m_id)tmp;
}


void m_free(m_id ptr, m_err_code* error) {
    struct mem_chunk** tmp=ptr;//указатель на указаель чтобы можно было сам указатель указывать на NULL
    if(*tmp==NULL){//проверка на то что мы удаляли
        printf("%s","DEALLOCATED ALREADY");
        *error=M_ERR_ALREADY_DEALLOCATED;
        return;
    }
    int begin_of_chunk=0;//переменная для сдвига
    _g_bytes_allocated -= (*tmp)->size;
    struct mem_chunk*next=(*tmp)->next;//удаление из стека
    struct mem_chunk*prev=(*tmp)->prev;//
    prev->next=next;//
    next->prev = prev;//
    free(*tmp);
    *tmp = NULL;
    struct mem_chunk*it=mem_chunks;
    while (it!=NULL){//rewriting of the array exclude deallocated chunk
        memcpy(_g_allocator_memory + begin_of_chunk, it->ptr, it->size);
        it->ptr = _g_allocator_memory + begin_of_chunk;
        begin_of_chunk += it->size;
        it = it->next;
    }


}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
    struct mem_chunk* tmp=read_from_id;
    if(tmp==NULL){
        *error=M_ERR_INVALID_CHUNK;
        return;
    }

    if(tmp->size>size_to_read){
        *error=M_ERR_OUT_OF_BOUNDS;
        return;
    }
    memcpy(read_to_buffer, tmp->ptr, size_to_read);
    *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    struct mem_chunk* tmp=write_to_id;
    if(tmp->size<size_to_write){
        *error=M_ERR_OUT_OF_BOUNDS;
        return;
    }
    memcpy(tmp->ptr, write_from_buffer, size_to_write);
    *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);
  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;
  mem_chunks=NULL;

}
