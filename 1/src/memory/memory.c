#include <stdlib.h>
#include <string.h>
#include <printf.h>

#include "memory.h"

char *_g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int current_chunk;
Pages* pages_table;

Chunks* chunks_table;

void read_from_chunk(m_id chunk_id, void* read_to_buffer, int size_to_read) {
    do {
        if (chunks_table->first_bit_of_chunk == chunk_id) {
            strncpy(chunks_table->data, read_to_buffer, size_to_read);
        }
        chunks_table = chunks_table->next;
    } while (chunks_table != NULL);
}

bool is_enough_written_in_chunk(int size_to_read, m_id chunk_id) {
    do {
        if (chunks_table->first_bit_of_chunk == chunk_id) {
            if (strlen(chunks_table->data) >= size_to_read) return true; else return false;
        }
        chunks_table = chunks_table->next;
    } while (chunks_table != NULL);
}

void free_chunk(m_id chunk_to_free) {
    while (chunks_table->first_bit_of_chunk != chunk_to_free) // просматриваем список начиная с корня
    { // пока не найдем узел, предшествующий lst
        chunks_table = chunks_table->next;
    }
    struct Chunks *temp = chunks_table->next;
    chunks_table->next = temp->next; // переставляем указател
}

void fill_chunk(m_id chunk_id, void* write_from_buffer, int size_to_write) {

    do {
        if (chunks_table->first_bit_of_chunk == chunk_id) {
            chunks_table->is_taken = true;
            strncpy(chunks_table->data, write_from_buffer, size_to_write);
        }
        chunks_table = chunks_table->next;
    } while (chunks_table != NULL);
}

bool is_chunk_free(m_id chunk_id) {
   do {
        if (chunk_id == chunks_table->first_bit_of_chunk) {
            if (chunks_table->is_taken) return false; else return true;
        }
        chunks_table = chunks_table->next;
    } while (chunks_table != NULL);
}

bool is_chunk_valid(m_id chunk_id) {
    do {
        if (chunks_table->first_bit_of_chunk == chunk_id) return true;
        chunks_table = chunks_table->next;
    } while (chunks_table != NULL);
    return false;
}

bool is_enough_space_of_chunk(m_id chunk_id, int size_to_write) {
    int needed_number_of_pages;
    if (size_to_write % size_of_page == 0) needed_number_of_pages = size_to_write / size_of_page; else needed_number_of_pages = size_to_write / size_of_page + 1;
    do {
        if (chunks_table->first_bit_of_chunk == chunk_id) {
            if (chunks_table->size >= needed_number_of_pages) return true; else return false;
        }
        chunks_table = chunks_table->next;
    } while (chunks_table != NULL);
}

struct Pages* append_chunk(struct Chunk** headRef, char* first_bit_of_chunk, int size)
{
    struct Chunks* current = *headRef;
    struct Chunks* newNode;
    newNode = malloc(sizeof(struct Chunks));
    newNode->is_taken = false;
    newNode->size = size;
    newNode->first_bit_of_chunk = first_bit_of_chunk;
    newNode->next = NULL;
    // если список пуст
    if (current == NULL) {
        *headRef = newNode;
    }
    else {
        // иначе
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newNode;
    }
    return  current;
}

m_id get_first_bit_of_free_pages(int needed_number_of_pages){
    int counter = needed_number_of_pages;
    int i = 0;
    do {
        if (counter != 0){
            if (pages_table->is_taken){
                counter = needed_number_of_pages;
                pages_table = pages_table + i;
            }
            else {
                counter--;
                pages_table = pages_table + i;
            }
        } else {
//            chunks_table = append_chunk(chunks_table, pages_table->first_bit - needed_number_of_pages * size_of_page ,needed_number_of_pages);
              if (current_chunk == 0) chunks_table = malloc(sizeof(struct Chunks)); else chunks_table = realloc(chunks_table, sizeof(struct Chunks));
            chunks_table->is_taken = false;
            chunks_table -> size = needed_number_of_pages;
            chunks_table -> first_bit_of_chunk = _g_allocator_memory + current_chunk;
            chunks_table -> data = malloc(size_of_page*needed_number_of_pages);
            current_chunk++;
            return current_chunk - 1 + _g_allocator_memory;
        }
    } while (pages_table != NULL);
    return NULL;
}

m_id m_malloc(int size_of_chunk, m_err_code* error_code) {
    int needed_number_of_pages;
    if (size_of_chunk % size_of_page == 0) needed_number_of_pages = size_of_chunk / size_of_page; else needed_number_of_pages = size_of_chunk / size_of_page + 1;
    if (get_first_bit_of_free_pages(needed_number_of_pages) != NULL){
        error_code = M_ERR_OK;
        return get_first_bit_of_free_pages(needed_number_of_pages);
    } else {
        error_code = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

}


void m_free(int ptr, m_err_code* error_code) {
    if (!is_chunk_valid(ptr)) {
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }
    if (is_chunk_free(ptr)) {
        *error_code = M_ERR_ALREADY_DEALLOCATED;
        return;
    }
    while (chunks_table->first_bit_of_chunk != ptr){
        chunks_table = chunks_table->next;
    }
    free_chunk(chunks_table);
}


void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error_code) {
    if (!is_chunk_valid(read_from_id)) {
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }
    if (!is_enough_space_of_chunk(read_from_id, size_to_read)) {
        *error_code = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    if (!is_enough_written_in_chunk(read_from_id, size_to_read)) {
        *error_code = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    read_from_chunk(read_from_id, read_to_buffer, size_to_read);
    *error_code = M_ERR_OK;
}

    void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error_code) {
        if (size_to_write != strlen(write_from_buffer)) {
            *error_code = M_ERR_INVALID_CHUNK;
            return;
        }
        if (!is_chunk_valid(write_to_id)) {
            *error_code = M_ERR_INVALID_CHUNK;
            return;
        }
        if (!is_enough_space_of_chunk(write_to_id, size_to_write)) {
            *error_code = M_ERR_OUT_OF_BOUNDS;
            return;
        }
        if (!is_chunk_free(write_to_id)) {
            *error_code = M_ERR_INVALID_CHUNK;
            return;
        }
        fill_chunk(write_to_id, write_from_buffer, size_to_write);
        *error_code = M_ERR_OK;
}

void listAllBlocks() {
    do {
        printf("%d ", chunks_table->data);
        chunks_table = chunks_table->next;
    } while (chunks_table != NULL);
}

void m_init(int num_of_pages, int s_of_page) {
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);
    number_of_pages =  num_of_pages;
    size_of_page = s_of_page;
    _g_allocator_memory_size = number_of_pages * size_of_page;
    _g_allocator_memory = malloc(_g_allocator_memory_size);

    pages_table = malloc(number_of_pages * sizeof(struct Pages));

    for(int i = 0; i < number_of_pages; i++){
        (pages_table + i) -> is_taken = false;
        (pages_table + i) -> first_bit = _g_allocator_memory + (i * size_of_page);
    }
}
