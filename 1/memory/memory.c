#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "memory.h"

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

int page_size = 0;
struct page *first_page;

m_id m_malloc(int size_of_chunk, m_err_code* error) {
  struct page* page = first_page;
  while(page -> next != NULL){
    if(page -> is_free == true){
      if(size_of_chunk <= page_size){
        page -> is_free = false;
        printf("Malloc complite\n");
        *error = M_ERR_OK;
        return page -> memory;
      }
      else{
        struct page* temp_page = page;
        int previous_page_count = 1;
        while(temp_page -> next -> is_free == true){
          temp_page = temp_page -> next;
          previous_page_count++;
          if(size_of_chunk <= page_size * previous_page_count){     
            for(int i = 0; i < previous_page_count; i++){           
              page -> is_free = false;                              
              if(i != 0) 
                page -> previous = temp_page; 
              temp_page = page;
              page = page -> next;
            }
            printf("Malloc complite\n");
            *error = M_ERR_OK;
            return page -> memory;
          }
        }
      }
    }
    page = page -> next;
  }
  *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
  return NULL;
}

void m_free(m_id ptr, m_err_code* error) {
  struct page* page = first_page;
  while(page -> next != NULL){
    m_id t = page -> memory;
    if(page -> memory == ptr){
      page -> is_free = true;
      while(page -> previous != NULL){
        page = page -> previous;
        page -> is_free = true;
      }
      printf("Deleted\n");
      *error = M_ERR_OK;
      return;
    }
    page = page -> next;
  }
}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
  memcpy(read_to_buffer, read_from_id, size_to_read);
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
  memcpy(write_to_id, write_from_buffer, size_to_write);
  *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = size_of_page;

  page_size = size_of_page;

  struct page* page;
  for(int i = 0; i < number_of_pages; i++){
    if (i == 0){
      page = (struct page*)malloc(sizeof(struct page));
      first_page = page;
    }
    else 
      page = page -> next;

    page -> memory = _g_allocator_memory + _g_bytes_allocated;
    page -> size = page_size;
    page -> is_free = true;
    page -> previous = NULL;

    if(i == number_of_pages - 1) 
      page -> next = NULL; 
    else 
      page -> next = (struct page*)malloc(sizeof(struct page));
    _g_bytes_allocated += page_size;
  }
}