#include <stdlib.h>
#include <string.h>
#include "memory.h"

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

typedef struct block {
    void* data;
    int data_size;
    int size;
    struct block* next;
} block;

typedef struct page {
    int total_size;
    int reserved_size;
    int is_reserved;
    block* block;
    struct page* next;
    int num_blocks;
} page;

typedef struct segment {
    page* first_page;
    int num_pages;
    struct segment* next;
} segment;

typedef struct {
    segment* segment;
    int num_segments;
} segment_table;

typedef struct {
    int num_of_pages_per_segment;
    int page_size;
    segment_table* segment_table;
} memory;

static memory* mem;

void set_values_to_block(page* page, block* block, int size_of_chunk) {         //добавляем блок на страницу
    block->size = size_of_chunk;
    page->reserved_size += size_of_chunk;
    page->num_blocks++;
}

m_id add_new_block(m_err_code* error, page* page, block* block, int size_of_chunk) {        //создание блока (выделение памяти + добавление на страницу)
    block = calloc(1, sizeof(struct block));       
    m_id ptr = (m_id)block;
    page->is_reserved = 1;
    set_values_to_block(page, block, size_of_chunk);
    *error = M_ERR_OK;
    return ptr;
}


m_id add_first_block(m_err_code* error, page* page, int size_of_chunk) {        //создание первого блока (выделение памяти + добавление на страницу)
    page->block = calloc(1, sizeof(block));
    m_id ptr = (m_id)page->block;
    page->is_reserved = 1;
    set_values_to_block(page, page->block, size_of_chunk);
    *error = M_ERR_OK;
    return ptr;
}

page* init_segment_pages(segment* segment) {        //выделение памяти под страницы
    segment->first_page = calloc(1, sizeof(page));
    segment->num_pages++;

    page* temp = segment->first_page;
    temp->total_size = mem->page_size;
    for (int i = 1; i < mem->num_of_pages_per_segment; ++i) {
        page* next = calloc(1, sizeof(page));
        next->total_size = mem->page_size;
        temp->next = next;
        temp = temp->next;
        mem->segment_table->segment->num_pages++;
    }
    return segment->first_page;
}

page* find_page_with_block(m_id ptr) {      //поиск страницы по заданному блоку
    segment* temp_segment = mem->segment_table->segment;
    for (int i = 0; i < mem->segment_table->num_segments; ++i) {
        page* temp_page = temp_segment->first_page;
        for (int j = 0; j < mem->num_of_pages_per_segment; j++) {
            block* temp_block = temp_page->block;
            if (temp_block == NULL) {
                temp_page = temp_page->next;
                continue;
            }
            while (strcmp((char*)temp_block, (char*)ptr) != 0) {
                temp_block = temp_block->next;
                if (temp_block == NULL) {
                    temp_page = temp_page->next;
                    break;
                }
            }
            if (temp_block == NULL) {
                continue;
            }
            return temp_page;
        }
        temp_segment = temp_segment->next;
    }
    return NULL;
}

block* find_block(m_id ptr) {       //поиск заданного блока
    page* page = find_page_with_block(ptr);
    if (page == NULL) {
        return NULL;
    }
    block* temp_block = page->block;
    while (strcmp((char*)temp_block, (char*)ptr) != 0) {
        temp_block = temp_block->next;
    }
    return temp_block;
}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
    if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    _g_bytes_allocated += size_of_chunk;

    segment* temp_segment = mem->segment_table->segment;
    segment* prev_segment = temp_segment;
    memory* m = mem;
    for (int i = 0; i < mem->segment_table->num_segments; ++i) {
        page* temp_page = temp_segment->first_page;
        for (int j = 0; j < mem->num_of_pages_per_segment; j++) {
            if (temp_page->is_reserved == 1) {
                temp_page = temp_page->next;
                continue;
            }

            if (temp_page->block == NULL) {     //выделяем память под первый блок
                return add_first_block(error, temp_page, size_of_chunk);
            }

            block* temp_block = temp_page->block;
            while (temp_block->next != NULL) {      //доходим до последнего блока и создаем новый после него
                temp_block = temp_block->next;
            }
            return add_new_block(error, temp_page, temp_block->next, size_of_chunk);
        }
        prev_segment = temp_segment;
        temp_segment = temp_segment->next;
    }

    segment* new_segment = calloc(1, sizeof(segment));      //если не хватает места на предыдущем сегменте создаем новый
    prev_segment->next = new_segment;
    mem->segment_table->num_segments++;

    page* temp_page = init_segment_pages(new_segment);
    return add_first_block(error, temp_page, size_of_chunk);        //выделяем память под первый блок
}


void m_free(m_id ptr, m_err_code* error) {
  page* page = find_page_with_block(ptr);
  if (page == NULL) {       //проверка на наличие страницы
      *error = M_ERR_INVALID_CHUNK;
      return;
  }

  block* cur_block = page->block;
  if (strcmp((char*)cur_block, (char*)ptr) == 0) {      //если текущий блок и блок ptr равны, то удаляем текущий блок
      block* next_block = cur_block->next;
      page->block = next_block;
      page->reserved_size -= cur_block->size;
      free(cur_block);
      *error = M_ERR_OK;
      return;
  }

  block* prev_block = cur_block;        //проверка всех остальных блоков
  cur_block = cur_block->next;
  while (strcmp((char*)cur_block, (char*)ptr) != 0) {
      cur_block = cur_block->next;
      prev_block = prev_block->next;
  }

  prev_block->next = cur_block->next;
  free(cur_block->data);
  page->reserved_size -= cur_block->size;
  free(cur_block);
  *error = M_ERR_OK;
}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
  block* block = find_block(read_from_id);
  if (block == NULL) {      //проверка на наличие блока
      *error = M_ERR_INVALID_CHUNK;
      return;
  }
  if (block->size < size_to_read) {     //проверка блока на подходящий размер
      *error = M_ERR_INVALID_CHUNK;
      return;
  }
  memcpy(read_to_buffer, block->data, size_to_read);        //запись данных в буфер из блока
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {

  block* block = find_block(write_to_id);
  if (block == NULL) {      //проверка на наличие блока
      *error = M_ERR_INVALID_CHUNK;
      return;
  }

  if (block->size < size_to_write) {        //проверка блока на подходящий размер
      *error = M_ERR_INVALID_CHUNK;
      return;
  }

  void* data = malloc(size_to_write);       //выделение памяти для данных с буфера
  memcpy(data, write_from_buffer, size_to_write);       //запись данных из буфера в data
  block->data = data;
  block->data_size = size_to_write;
  *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;

  mem = calloc(1, sizeof(memory));      //выделение общей памяти
  mem->segment_table = calloc(1, sizeof(segment_table));        //выделение памяти под таблицу сегментов
  mem->segment_table->segment = calloc(1, sizeof(segment));     //выделение памяти под сегмент
  mem->segment_table->num_segments++;
  mem->page_size = size_of_page;
  mem->num_of_pages_per_segment = number_of_pages;

  init_segment_pages(mem->segment_table->segment);        //выделение памяти под страницы
}
