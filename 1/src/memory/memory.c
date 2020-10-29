#include <stdlib.h>
#include <string.h>

#include "memory.h"

const unsigned CACHE_SIZE_IN_PAGES = 8;
const unsigned LOCALITY_SIZE = 2;
const unsigned PAGE_SIZE_IN_BYTES = 16;

struct MainMemoryId {
    unsigned fromLinearAddress;
    unsigned sizeInBytes;
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

struct CacheMemoryNode _g_cache_table[CACHE_SIZE_IN_PAGES];
struct MainMemoryNode* _g_main_memory_table;
char* _g_cache_memory;
char* _g_main_memory;

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

}


void m_free(m_id ptr, m_err_code* error) {

}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {

}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {

}


void m_init(int number_of_pages, int size_of_page) {

}
