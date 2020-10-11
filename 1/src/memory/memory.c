#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "memory.h"

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;
struct mem_chunk* chunks_list[50];//array of allocated chunks
//struct mem_chunk* deleted_chunks[50];//array of deallocated chunks
int size=0;//size of chunks_list array
//int deleted_size=0;//size of deleted_chunks array

void debug(){
        printf("%s\n",_g_allocator_memory);
        printf("%s\n",_g_allocator_memory+chunks_list[0]->size);
        printf("%s\n",_g_allocator_memory+chunks_list[0]->size+chunks_list[1]->size);

}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
  if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
    return NULL;
  }
  struct mem_chunk* tmp=malloc(sizeof(struct mem_chunk));
  tmp->ptr=_g_allocator_memory+_g_bytes_allocated;
  tmp->size=size_of_chunk;
  chunks_list[size]=tmp;
  size++;
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

    if(chunks_list[size] == *tmp)//если блок в массиве последний
    {
        free(*tmp);
        *tmp=NULL;
        return;

    }


    for(int i=0; i < size; i++)
    {
        if(chunks_list[i]==*tmp)//если нашли блок который нужно удалить
        {
            free(chunks_list[i]);
            for(int j=i; j < size - 1; j++){
                memcpy(_g_allocator_memory+begin_of_chunk,chunks_list[j+1]->ptr,chunks_list[j+1]->size);//перезапись
                chunks_list[j+1]->ptr=_g_allocator_memory+begin_of_chunk;//изменение указателя следующего элемента на текущее место
                begin_of_chunk+=chunks_list[j+1]->size;//увеличение сдвига
                chunks_list[j]=chunks_list[j+1];//фактически сдиг массива
                chunks_list[j+1]=NULL;//чтобы когда массив уменьшился на 1 size+1 элемент которого какбы нет не был равен size элементу

            }
            size--;//уменьшение размера массива
            *tmp=NULL;//изменние указателя который на входе чтобы понятно было что он удален
            return;

        }else{
            begin_of_chunk+=chunks_list[i]->size;//рассчитываем начальный сдвиг
        }
    }
    *error = M_ERR_OK;
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
}
