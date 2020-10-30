#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "memory.h"

#define CACHE_SIZE_IN_PAGES 8
#define LOCALITY_SIZE 2
#define PAGE_SIZE_IN_BYTES 16

struct MainMemoryId {
    char* fromPointer;
    unsigned sizeInBytes;
};

struct MainMemoryIdNode {
    struct MainMemoryId* mainMemoryId;
    struct MainMemoryIdNode* next;
};

struct MainMemoryNode {
    struct MainMemoryNode* previous;
    struct MainMemoryNode* next;
    struct MainMemoryId* mainMemoryId;
    unsigned fromPhysicalAddress;
    unsigned sizeInBytes;
};

struct CacheMemoryNode {
    struct MainMemoryId* mainMemoryId;
    unsigned cacheNodeNumber; //number of page in cache
    unsigned pageNumber; //number of page of the segment
    char flags; //XXXXXXXC, X - reserved for further standardization)), C - a flag which indicates whether the page was changed or not
};

struct MainMemoryIdNode* _g_main_memory_ids_table;

struct CacheMemoryNode _g_cache_table[CACHE_SIZE_IN_PAGES];
struct MainMemoryNode* _g_main_memory_table = NULL;
char _g_cache_memory[CACHE_SIZE_IN_PAGES * PAGE_SIZE_IN_BYTES];
char* _g_main_memory = NULL;

unsigned long _g_main_memory_size;
unsigned long _g_used_memory_size;

//flushes only one node of cache
void flush_cache_memory_node(struct CacheMemoryNode* cache_memory_node, char* cache_memory,
                             struct MainMemoryNode* main_memory_table, char* main_memory) {

}

//checks every node of cache and flushes it if it's needed
void flush_cache_where_required(struct CacheMemoryNode* cache_memory_table, unsigned cache_size_in_pages,
                                char* cache_memory, struct MainMemoryNode* main_memory_table, char* main_memory) {

}

//inserts a new node into the main memory table
void insert_new_memory_node(struct MainMemoryNode* main_memory_node_to_insert_after,
                            struct MainMemoryNode* main_memory_node_to_insert) {

}

//removes a node from the main memory table
void remove_main_memory_node(struct MainMemoryNode* main_memory_node_to_remove) {

}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
    if (_g_main_memory_size - _g_used_memory_size < size_of_chunk) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;

        return NULL;
    }

    struct MainMemoryId* main_memory_id = NULL;
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    unsigned lastToPhysicalAddress = 0;

    while (current_main_memory_node) {
        unsigned currentLinearAddress = current_main_memory_node->fromPhysicalAddress - lastToPhysicalAddress;

        if (currentLinearAddress > 0) {
            //then there's some free space, we are to insert a new main memory node here

        }

        current_main_memory_node = current_main_memory_node->next;
    }

    *error = M_ERR_OK;

    assert(main_memory_id != NULL);

    return main_memory_id->fromPointer;
}


void m_free(m_id ptr, m_err_code* error) {

}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {

}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {

}


void m_init(int number_of_pages, int size_of_page) {
    if (_g_main_memory) {
        //free the main memory
        free(_g_main_memory);

        struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

        //free the main memory nodes
        while (current_main_memory_node) {
            struct MainMemoryNode* next_main_memory_node = current_main_memory_node->next;

            free(current_main_memory_node);

            current_main_memory_node = next_main_memory_node;
        }

        struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

        //free ids' nodes and ids
        while (current_main_memory_id_node) {
            struct MainMemoryIdNode* next_main_memory_id_node = current_main_memory_id_node->next;

            free(current_main_memory_id_node->mainMemoryId);
            free(next_main_memory_id_node);

            current_main_memory_id_node = next_main_memory_id_node;
        }
    }

    _g_main_memory_size = number_of_pages * size_of_page;

    _g_main_memory = NULL;
    _g_main_memory_table = NULL;
    _g_main_memory_ids_table = NULL;
    _g_main_memory = malloc(_g_main_memory_size);
    _g_used_memory_size = 0;

    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES; ++i)
        _g_cache_table[i] = (struct CacheMemoryNode){NULL, 0, i, 0};
}
