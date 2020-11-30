#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <printf.h>

#define M_ERR_OK 0 // Everything is ok
#define M_ERR_ALLOCATION_OUT_OF_MEMORY 1 // Not enough memory for allocation
#define M_ERR_ALREADY_DEALLOCATED 2 // The chunk was already deallocated
#define M_ERR_INVALID_CHUNK 3 // The chunk is invalid, the operation did not succeed
#define M_ERR_OUT_OF_BOUNDS 4 // The read/write operation out of bounds

//-MARK:- Global variables and typedefs

int size_of_page;
int number_of_pages;
void *_g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;
int current_chunk = 0;

typedef int m_err_code; // Error code of sandbox memory
typedef void* m_id; // Identifier of sandbox memory chunk

//-MARK:- Used structures

typedef struct Chunks{
    void* first_bit_of_chunk;
    bool is_taken;
    int size;
    char * data;
}Chunks;

typedef struct Pages{
    bool is_taken;
    void* first_bit;
}Pages;

Pages pages_table[1000];
Chunks chunks_table[1000];

//-MARK:- Function inicialization

void m_init(int number_of_pages, int size_of_page);

m_id m_malloc(int size_of_chunk, m_err_code* error_code);

void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error_code);

void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error_code);

void m_free(m_id chunk_id, m_err_code* error_code);

//-MARK:- Main

int main(int argc, char **argv) {
    char buffer[50];
    m_init(10, 5);
    
    int error_code;
    m_id chunk_1 = m_malloc(51, &error_code);
    printf("%d\n", error_code);
    m_write(chunk_1, "1234", 4, &error_code);
    printf("%d\n", error_code);
    m_read(chunk_1, buffer, 3, &error_code);
    if (error_code == M_ERR_OK)  printf("%s\n", buffer);
    printf("%d\n", error_code);
    m_free(chunk_1, &error_code);
    printf("%d\n", error_code);
    
    m_id chunk_2 = m_malloc(10, &error_code);
    printf("%d\n", error_code);
    m_write(chunk_2, "1234", 4, &error_code);
    printf("%d\n", error_code);
    m_read(chunk_2, buffer, 3, &error_code);
    if (error_code == M_ERR_OK)  printf("%s\n", buffer);
    printf("%d\n", error_code);
    m_free(chunk_2, &error_code);
    printf("%d\n", error_code);

}

//-MARK:- Supporting functions

void read_from_chunk(m_id chunk_id, void* read_to_buffer, int size_to_read) {
    for(int i = 0; i < sizeof(chunks_table); i++){
        if (chunks_table[i].first_bit_of_chunk == chunk_id) {
            strncpy(read_to_buffer, chunks_table[i].data, size_to_read);
            break;
        }
    }
}

bool is_enough_written_in_chunk(m_id chunk_id, int size_to_read) {
    for(int i = 0; i < current_chunk + 1; i++){
        if (chunks_table[i].first_bit_of_chunk == chunk_id) {
            if (strlen(chunks_table[i].data) >= size_to_read) return true; else return false;
        }
    }
    return false;
}

void free_chunk(m_id chunk_to_free) {
    struct Chunks temp_chunk[1000];
    for(int i = 0; i < current_chunk + 1; i++){
        if(chunks_table[i].first_bit_of_chunk != chunk_to_free){
            temp_chunk[i] = chunks_table[i];
        } else {
            for (int j = 0; j < number_of_pages; j++) {
                if (pages_table[j].first_bit == chunk_to_free) {
                    for (int k = 0; k < chunks_table[i].size; k++) pages_table[j + k].is_taken = false;
                    break;
                }
            }
        }
    }
    current_chunk--;
}

void fill_chunk(m_id chunk_id, void* write_from_buffer, int size_to_write) {
    for(int i = 0; i < current_chunk + 1; i++){
        if (chunks_table[i].first_bit_of_chunk == chunk_id) {
            chunks_table[i].is_taken = true;
            strncpy(chunks_table[i].data, write_from_buffer, size_to_write);
            break;
        }
    }
}

bool is_chunk_free(m_id chunk_id) {
    for(int i = 0; i < current_chunk + 1; i++){
        if (chunks_table[i].first_bit_of_chunk == chunk_id) {
            if (chunks_table[i].is_taken) return false;
        }
    }
    return true;
}

bool is_chunk_exist(m_id chunk_id) {
    for(int i = 0; i < current_chunk + 1; i++){
        if (chunks_table[i].first_bit_of_chunk == chunk_id && chunk_id != NULL) return true;
    }
    return false;
}

bool is_enough_space_of_chunk(m_id chunk_id, int size_to_write) {
    int needed_number_of_pages;
    if (size_to_write % size_of_page == 0) needed_number_of_pages = size_to_write / size_of_page; else needed_number_of_pages = size_to_write / size_of_page + 1;
    for(int i = 0; i < sizeof(chunks_table); i++){
        if (chunks_table[i].first_bit_of_chunk == chunk_id) {
            if (chunks_table[i].size >= needed_number_of_pages) return true; else return false;
        }
    }
    return false;
}

m_id get_first_bit_of_free_pages(int needed_number_of_pages){
    int counter = needed_number_of_pages;
    for(int i = 0; i < number_of_pages; i++) {
        if (counter != 0){
            if (pages_table[i].is_taken){
                counter = needed_number_of_pages;
            }
            else {
                counter--;
            }
        } else {
            for(int j = i - needed_number_of_pages; j < i; j++){
                pages_table[j].is_taken = true;
            }
            chunks_table[current_chunk].is_taken = false;
            chunks_table[current_chunk].first_bit_of_chunk = pages_table[i - needed_number_of_pages ].first_bit;
            chunks_table[current_chunk].size = needed_number_of_pages;
            chunks_table[current_chunk].data = malloc(size_of_page * needed_number_of_pages);
            return chunks_table[current_chunk].first_bit_of_chunk;
        }
    }
    return NULL;
}

//-MARK:- Task functuions

m_id m_malloc(int size_of_chunk, m_err_code* error_code) {
    int needed_number_of_pages;
    if (size_of_chunk % size_of_page == 0) needed_number_of_pages = size_of_chunk / size_of_page; else needed_number_of_pages = size_of_chunk / size_of_page + 1;
    void* temp_first_bit_of_chunk = get_first_bit_of_free_pages(needed_number_of_pages);
    if (temp_first_bit_of_chunk != NULL){
        *error_code = M_ERR_OK;
        return temp_first_bit_of_chunk;
    } else {
        *error_code = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }
    
}

void m_free(m_id chunk_id, m_err_code* error_code) {
    if (!is_chunk_exist(chunk_id)) {
        *error_code = M_ERR_INVALID_CHUNK;
        return;
    }
    if (is_chunk_free(chunk_id)) {
        *error_code = M_ERR_ALREADY_DEALLOCATED;
        return;
    }
    free_chunk(chunk_id);
}


void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error_code) {
    if (!is_chunk_exist(read_from_id)) {
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
    if (!is_chunk_exist(write_to_id)) {
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

void m_init(int num_of_pages, int s_of_page) {
    
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);
    number_of_pages =  num_of_pages;
    size_of_page = s_of_page;
    _g_allocator_memory_size = number_of_pages * size_of_page;
    _g_allocator_memory = malloc(_g_allocator_memory_size);
    if(sizeof(pages_table) < num_of_pages){
        printf("YOU ARE OUTREACHED MEMORY SYSTEM BOUNDS!");
    }
    for(int i = 0; i < num_of_pages; i++){
        pages_table[i].is_taken = false;
        _g_allocator_memory += size_of_page;
        pages_table[i].first_bit = _g_allocator_memory;
    }
    
}
