#include "memory.h"
#include <stdlib.h>
#include <string.h>

typedef struct block {
    void* data;
    int data_size;
    int size;
    struct block* next;
} block;

typedef struct page {
    int total_size;
    int reserved_size;
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

void set_values_to_block(page* page, block* block, int size_of_block) {

    block->size = size_of_block;
    page->reserved_size += size_of_block;
    page->num_blocks++;
}

int add_new_block(m_id* chunk_id, page* page, block* block, int size_of_block) {

    block = calloc(1, sizeof(block));
    *chunk_id = (m_id)block;
    set_values_to_block(page, block, size_of_block);

    return M_ERR_OK;
}


int add_first_block(m_id* chunk_id, page* page, int size_of_block) {

    page->block = calloc(1, sizeof(block));
    *chunk_id = (m_id)page->block;
    set_values_to_block(page, page->block, size_of_block);

    return M_ERR_OK;
}

page* init_segment_pages(segment* segment) {

    segment->first_page = calloc(1, sizeof(page));
    segment->num_pages++;

    page* temp = mem->segment_table->segment->first_page;
    temp->total_size = mem->page_size;
    for (int i = 1; i < mem->num_of_pages_per_segment; i++) {

        page* next = calloc(1, sizeof(page));
        next->total_size = mem->page_size;
        temp->next = next;
        temp = temp->next;

        mem->segment_table->segment->num_pages++;
    }

    return segment->first_page;
}

int m_malloc(m_id* chunk_id, int size_of_block) {

    // случай когда блок в первой странице и есть место
    // случай когда блоку не хватает место в первой странице и переходим на новый
    // случай когда не хватает места в сегменте и создаем новый

    segment* temp_segment = mem->segment_table->segment;
    for (int i = 0; i < mem->segment_table->num_segments; i++) {

        page* temp_page = temp_segment->first_page;
        for (int j = 0; j < mem->num_of_pages_per_segment; j++) {

            if (temp_page->total_size - temp_page->reserved_size < size_of_block) {
                temp_page = temp_page->next;
                continue;
            }

            if (temp_page->block == NULL) {                                 // когда block первый
                return add_first_block(chunk_id, temp_page, size_of_block);
            }

            block* temp_block = temp_page->block;
            while (temp_block->next != NULL) {                              // Находим последний блок
                temp_block = temp_block->next;
            }

            return add_new_block(chunk_id, temp_page, temp_block->next, size_of_block);
        }

        temp_segment = temp_segment->next;
    }

    segment* new_segment = calloc(1, sizeof(segment));
    temp_segment->next = new_segment;
    mem->segment_table->num_segments++;

    page* temp_page = init_segment_pages(new_segment);

    return add_first_block(chunk_id, temp_page, size_of_block);
}

page* find_page_with_block(m_id chunk_id) {

    segment* temp_segment = mem->segment_table->segment;
    for (int i = 0; i < mem->segment_table->num_segments; i++) {

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

int m_free(m_id chunk_id) {

    page* page = find_page_with_block(chunk_id);
    if (page == NULL) {
        return M_ERR_BAD_PARAMS;
    }

    block* cur_block = page->block;
    if (strcmp((char*)cur_block, (char*)chunk_id) == 0) {
        block* next_block = cur_block->next;
        page->block = next_block;
        page->reserved_size -= cur_block->size;
        free(cur_block);

        return M_ERR_OK;
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

    return M_ERR_OK;
}

int m_read(m_id read_from_id, void* read_to_buffer, int size_to_read) {

    block* block = find_block(read_from_id);
    if (block == NULL || block->size < size_to_read) {
        return M_ERR_BAD_PARAMS;
    }

    memcpy(read_to_buffer, block->data, size_to_read);

    return M_ERR_OK;
}


int m_write(m_id write_to_id, void* write_from_buffer, int size_to_write) {

    block* block = find_block(write_to_id);
    if (block == NULL || block->size < size_to_write) {
        return M_ERR_BAD_PARAMS;
    }

    void* data = malloc(size_to_write);
    memcpy(data, write_from_buffer, size_to_write);

    block->data = data;
    block->data_size = size_to_write;

    return M_ERR_OK;
}


int m_init(int n, int szPage) {

    mem = calloc(1, sizeof(memory));
    if (mem == NULL) {
        return M_ERR_UNKNOWN;
    }

    mem->segment_table = calloc(1, sizeof(segment_table));
    if (mem->segment_table == NULL) {
        return M_ERR_UNKNOWN;
    }

    mem->segment_table->segment = calloc(1, sizeof(segment));
    if (mem->segment_table->segment == NULL) {
        return M_ERR_UNKNOWN;
    }

    mem->segment_table->num_segments++;
    mem->page_size = szPage;
    mem->num_of_pages_per_segment = n;
    init_segment_pages(mem->segment_table->segment);

    return M_ERR_OK;
}


int m_clean() {

    segment* cur_segment = mem->segment_table->segment;
    segment* next_segment = mem->segment_table->segment;
    for (int i = 0; i < mem->segment_table->num_segments; i++) {

        page* cur_page = cur_segment->first_page;
        page* next_page = NULL;
        if (cur_page->next != NULL) {
            next_page = cur_segment->first_page->next;
        }

        for (int j = 0; j < mem->num_of_pages_per_segment; j++) {
            free(cur_page);
            cur_page = next_page;
            if (cur_page != NULL) {
                next_page = cur_page->next;
            }
        }
        free(cur_segment);

        cur_segment = next_segment;
        if (cur_segment != NULL) {
            next_segment = cur_segment->next;
        }
    }

    free(mem->segment_table);
    free(mem);

    return M_ERR_OK;
}