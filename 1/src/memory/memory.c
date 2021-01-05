#include <stdlib.h>
#include <string.h>

#include "memory.h"

typedef char *VA;

typedef struct block {
    void *data;
    size_t data_size;
    size_t size;
    struct block *next;
} block;

typedef struct page {
    size_t total_size;
    size_t reserved_size;
    int8_t is_reserved;
    block *block;
    struct page *next;
    size_t num_blocks;
} page;


typedef struct segment {
    page *first_page;
    size_t num_pages;
    struct segment *next;
} segment;

typedef struct {
    segment *segment;
    size_t num_segments;
} segment_table;

typedef struct {
    int num_of_pages_per_segment;
    size_t page_size;
    segment_table *segment_table;
} memory;

static memory *mem;

char *_g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

void set_values_to_block(page *page, block *block, size_t szBlock) {
    block->size = szBlock;
    page->reserved_size += szBlock;
    page->num_blocks++;
}

m_id add_first_block(m_err_code *error, page *page, size_t szBlock) {
    page->block = calloc(1, sizeof(block));
    page->is_reserved = 1;
    set_values_to_block(page, page->block, szBlock);
    error = M_ERR_OK;
    return (VA) page->block;
}

m_id add_new_block(m_err_code *error, page *page, block *block, size_t szBlock) {
    block = calloc(1, sizeof(struct block));
    page->is_reserved = 1;
    set_values_to_block(page, block, szBlock);
    error = M_ERR_OK;
    return (VA) block;
}

page *init_segment_pages(segment *segment) {
    segment->first_page = calloc(1, sizeof(page));
    segment->num_pages++;

    page *temp = segment->first_page;
    temp->total_size = mem->page_size;
    for (int i = 1; i < mem->num_of_pages_per_segment; ++i) {
        page *next = calloc(1, sizeof(page));
        next->total_size = mem->page_size;
        temp->next = next;
        temp = temp->next;
        mem->segment_table->segment->num_pages++;
    }
    return segment->first_page;
}

m_id m_malloc(int size_of_chunk, m_err_code *error) {
    segment *temp_segment = mem->segment_table->segment;
    segment *prev_segment = temp_segment;
    memory *m = mem;
    for (size_t i = 0; i < mem->segment_table->num_segments; ++i) {
        page *temp_page = temp_segment->first_page;
        for (size_t j = 0; j < mem->num_of_pages_per_segment; j++) {
            if (temp_page->is_reserved == 1) {
                temp_page = temp_page->next;
                continue;
            }

            if (temp_page->block == NULL) {
                return add_first_block(error, temp_page, size_of_chunk);
            }

            block *temp_block = temp_page->block;
            while (temp_block->next != NULL) {
                temp_block = temp_block->next;
            }
            return add_new_block(error, temp_page, temp_block->next, size_of_chunk);
        }
        prev_segment = temp_segment;
        temp_segment = temp_segment->next;
    }

    segment *new_segment = calloc(1, sizeof(segment));
    prev_segment->next = new_segment;
    mem->segment_table->num_segments++;

    page *temp_page = init_segment_pages(new_segment);
    return add_first_block(error, temp_page, size_of_chunk);
}

page *find_page_with_block(VA ptr) {
    segment *temp_segment = mem->segment_table->segment;
    for (size_t i = 0; i < mem->segment_table->num_segments; ++i) {
        page *temp_page = temp_segment->first_page;
        for (size_t j = 0; j < mem->num_of_pages_per_segment; j++) {
            block *temp_block = temp_page->block;
            if (temp_block == NULL) {
                temp_page = temp_page->next;
                continue;
            }
            while (strcmp((char *) temp_block, (char *) ptr) != 0) {
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

block *find_block(VA ptr) {
    page *page = find_page_with_block(ptr);
    if (page == NULL) {
        return NULL;
    }
    block *temp_block = page->block;
    while (strcmp((char *) temp_block, (char *) ptr) != 0) {
        temp_block = temp_block->next;
    }
    return temp_block;
}

//todo last stop
void m_free(m_id ptr, m_err_code *error) {
    page *page = find_page_with_block((VA) ptr);
    if (page == NULL) {
        error = M_ERR_ALREADY_DEALLOCATED;
    }
    block *cur_block = page->block;
    if (strcmp((char *) cur_block, (char *) ptr) == 0) {
        block *next_block = cur_block->next;
        page->block = next_block;
        page->reserved_size -= cur_block->size;
        free(cur_block);
        error = M_ERR_OK;
    }
    block *prev_block = cur_block;
    cur_block = cur_block->next;
    while (strcmp((char *) cur_block, (char *) ptr) != 0) {
        cur_block = cur_block->next;
        prev_block = prev_block->next;
    }
    prev_block->next = cur_block->next;
    free(cur_block->data);
    page->reserved_size -= cur_block->size;
    free(cur_block);
    error = M_ERR_OK;
}

void m_read(m_id read_from_id, void *read_to_buffer, int size_to_read, m_err_code *error) {
    block *block = find_block(read_from_id);
    if (block == NULL) {
        error = M_ERR_INVALID_CHUNK;
    }
    if (block->size < size_to_read) {
        error = M_ERR_OUT_OF_BOUNDS;
    }
    memcpy(read_to_buffer, block->data, size_to_read);
    error = M_ERR_OK;
}

void m_write(m_id write_to_id, void *write_from_buffer, int size_to_write, m_err_code *error) {
    block *block = find_block((VA) write_to_id);
    if (block == NULL) {
        error = M_ERR_INVALID_CHUNK;
    }

    if (block->size < size_to_write) {
        error = M_ERR_OUT_OF_BOUNDS;
    }

    void *data = malloc(size_to_write);
    memcpy(data, write_from_buffer, size_to_write);
    block->data = data;
    block->data_size = size_to_write;
    error = M_ERR_OK;
}

void m_init(int number_of_pages, int size_of_page) {
    mem = calloc(1, sizeof(memory));
    mem->segment_table = calloc(1, sizeof(segment_table));
    mem->segment_table->segment = calloc(1, sizeof(segment));
    mem->segment_table->num_segments++;
    mem->page_size = size_of_page;
    mem->num_of_pages_per_segment = number_of_pages;
    init_segment_pages(mem->segment_table->segment);
}