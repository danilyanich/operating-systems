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

void set_values_to_block(page* page, block* block, int size_of_chunk) {        
    block->size = size_of_chunk;
    page->reserved_size += size_of_chunk;
    page->num_blocks++;
}

m_id add_new_block(m_err_code* error_code, page* page, block* block, int size_of_chunk) {
    block = calloc(1, sizeof(struct block));
    m_id ptr = (m_id)block;
    page->is_reserved = 1;
    set_values_to_block(page, block, size_of_chunk);
    *error_code = M_ERR_OK;
    return ptr;
}


m_id add_first_block(m_err_code* error_code, page* page, int size_of_chunk) {       
    page->block = calloc(1, sizeof(block));
    m_id ptr = (m_id)page->block;
    page->is_reserved = 1;
    set_values_to_block(page, page->block, size_of_chunk);
    *error_code = M_ERR_OK;
    return ptr;
}

page* init_segment_pages(segment* segment) {        
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

page* find_page_with_block(m_id chunk_id) {      
    segment* temp_segment = mem->segment_table->segment;
    for (int i = 0; i < mem->segment_table->num_segments; ++i) {
        page* temp_page = temp_segment->first_page;
        for (int j = 0; j < mem->num_of_pages_per_segment; j++) {
            block* temp_block = temp_page->block;
            if (temp_block == NULL) {
                temp_page = temp_page->next;
                continue;
            }
            while (strcmp((char*)temp_block, (char*)chunk_id) != 0) {
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

block* find_block(m_id chunk_id) {     
    page* page = find_page_with_block(chunk_id);
    if (page == NULL) {
        return NULL;
    }
    block* temp_block = page->block;
    while (strcmp((char*)temp_block, (char*)chunk_id) != 0) {
        temp_block = temp_block->next;
    }
    return temp_block;
}

m_id m_malloc(int size_of_chunk, m_err_code* error_code) {
    if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
        *error_code = M_ERR_ALLOCATION_OUT_OF_MEMORY;
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

            if (temp_page->block == NULL) {     
                return add_first_block(error_code, temp_page, size_of_chunk);
            }

            block* temp_block = temp_page->block;
            while (temp_block->next != NULL) {      
                temp_block = temp_block->next;
            }
            return add_new_block(error_code, temp_page, temp_block->next, size_of_chunk);
        }
        prev_segment = temp_segment;
        temp_segment = temp_segment->next;
    }

    segment* new_segment = calloc(1, sizeof(segment));     
    prev_segment->next = new_segment;
    mem->segment_table->num_segments++;

    page* temp_page = init_segment_pages(new_segment);
    return add_first_block(error_code, temp_page, size_of_chunk);       
}


void m_free(m_id chunk_id, m_err_code* error_code) {
    page* page = find_page_with_block(chunk_id);
    if (page == NULL) {     
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }

    block* cur_block = page->block;
    if (strcmp((char*)cur_block, (char*)chunk_id) == 0) {     
        block* next_block = cur_block->next;
        page->block = next_block;
        page->reserved_size -= cur_block->size;
        free(cur_block);
        *error_code = M_ERR_OK;
        return;
    }

    block* prev_block = cur_block;        
    cur_block = cur_block->next;
    while (strcmp((char*)cur_block, (char*)chunk_id) != 0) {
        cur_block = cur_block->next;
        prev_block = prev_block->next;
    }

    prev_block->next = cur_block->next;
    free(cur_block->data);
    page->reserved_size -= cur_block->size;
    free(cur_block);
    *error_code = M_ERR_OK;
}


void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error_code) {
    block* block = find_block(read_from_id);
    if (block == NULL) {      
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }
    if (block->size < size_to_read) {     
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }
    memcpy(read_to_buffer, block->data, size_to_read);       
    *error_code = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error_code) {

    block* block = find_block(write_to_id);
    if (block == NULL) {     
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }

    if (block->size < size_to_write) {       
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }

    void* data = malloc(size_to_write);      
    memcpy(data, write_from_buffer, size_to_write);       
    block->data = data;
    block->data_size = size_to_write;
    *error_code = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);

    _g_allocator_memory_size = number_of_pages * size_of_page;
    _g_allocator_memory = malloc(_g_allocator_memory_size);
    _g_bytes_allocated = 0;

    mem = calloc(1, sizeof(memory));     
    mem->segment_table = calloc(1, sizeof(segment_table));        
    mem->segment_table->segment = calloc(1, sizeof(segment));    
    mem->segment_table->num_segments++;
    mem->page_size = size_of_page;
    mem->num_of_pages_per_segment = number_of_pages;

    init_segment_pages(mem->segment_table->segment);      
}