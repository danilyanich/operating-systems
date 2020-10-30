#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "memory.h"

#define CACHE_SIZE_IN_PAGES 8
#define LOCALITY_SIZE 2
#define PAGE_SIZE_IN_BYTES 16

struct MainMemoryIdNode {
    struct MainMemoryIdNode* previous;
    char* fromLinearAddressPointer;
    unsigned sizeInBytes;
    struct CachedPage* cachedPages;
};

struct MainMemoryNode {
    struct MainMemoryNode* previous;
    struct MainMemoryNode* next;
    struct MainMemoryIdNode* mainMemoryIdNode;
    unsigned fromPhysicalAddress;
    unsigned sizeInBytes;
};

struct CachedPage {
    struct CachedPage* next;
    unsigned cachePageNumber; //number of page in cache
    unsigned pageNumber; //number of page of the segment
    char flags; //XXXXXXXC, X - reserved for further standardization)), C - a flag which indicates whether the page was changed or not
};

struct MainMemoryIdNode* _g_main_memory_ids_table;

char _g_cache_usage_table[CACHE_SIZE_IN_PAGES / sizeof(char)];
struct MainMemoryNode* _g_main_memory_table = NULL;
char _g_cache_memory[CACHE_SIZE_IN_PAGES * PAGE_SIZE_IN_BYTES];
char* _g_main_memory = NULL;

unsigned long _g_main_memory_size;
unsigned long _g_used_memory_size;

//flushes only one node of cache
void flush_cache_memory_node(struct CachedPage* cache_memory_node, char* cache_memory,
                             struct MainMemoryNode* main_memory_table, char* main_memory) {

}

//checks every node of cache and flushes it if it's needed
void flush_cache_where_required(struct CachedPage* cache_memory_table, unsigned cache_size_in_pages,
                                char* cache_memory, struct MainMemoryNode* main_memory_table, char* main_memory) {

}

//inserts a new node into the main memory table
void insert_new_memory_node(struct MainMemoryNode* main_memory_anchor_node,
                            struct MainMemoryNode* main_memory_node_to_insert, char insertAfterAnchor) {
    struct MainMemoryNode* anotherSide;

    if (insertAfterAnchor) {
        //after
        anotherSide = main_memory_anchor_node->next;

        main_memory_anchor_node->next = main_memory_node_to_insert;

        main_memory_node_to_insert->previous = main_memory_anchor_node;
        main_memory_node_to_insert->next = anotherSide;

        anotherSide->previous = main_memory_node_to_insert;
    } else {
        //before (it's only allowed to be used when there's nothing before the anchor (it's the beginning))
        main_memory_anchor_node->previous = main_memory_node_to_insert;

        main_memory_node_to_insert->previous = NULL;
        main_memory_node_to_insert->next = main_memory_anchor_node;

        _g_main_memory_table = main_memory_node_to_insert;
    }
}

//removes a node from the main memory table
void remove_main_memory_node(struct MainMemoryNode* main_memory_node_to_remove) {

}

unsigned obtain_linear_address_for_chunk(unsigned size_of_chunk) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

    if (!current_main_memory_id_node) {
        return 0;
    } else {
        unsigned maximalAddress =
                current_main_memory_id_node->fromLinearAddressPointer - (char*)0 + current_main_memory_id_node->sizeInBytes;
        unsigned lastFrom = current_main_memory_id_node->fromLinearAddressPointer - (char*)0;

        while (current_main_memory_id_node) {
            unsigned currentTo = current_main_memory_id_node->fromLinearAddressPointer - (char*)0;
            long gap =
                    (long)lastFrom - (currentTo + current_main_memory_id_node->sizeInBytes);

            if (gap > size_of_chunk)
                return currentTo;

            current_main_memory_id_node = current_main_memory_id_node->previous;
        }

        return maximalAddress;
    }
}

void allocate_chunk(unsigned size_of_chunk, struct MainMemoryIdNode* main_memory_id_node) {
    //create enough count of main memory nodes to fit the chunk in them
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;
    struct MainMemoryNode* last_main_memory_node = NULL;

    unsigned memory_to_allocate = size_of_chunk;
    unsigned last_physical_address = 0;

    while (memory_to_allocate > 0) {
        unsigned gap_from_last_node = current_main_memory_node
                                      ? current_main_memory_node->fromPhysicalAddress - last_physical_address
                                      : ~0u;

        if (gap_from_last_node > 0) {
            //then there's some free space, we are to insert a new main memory node here
            unsigned nodeSize = gap_from_last_node >= memory_to_allocate
                                ? memory_to_allocate
                                : gap_from_last_node;

            memory_to_allocate -= nodeSize;

            struct MainMemoryNode* new_main_memory_node = malloc(sizeof(struct MainMemoryNode));

            new_main_memory_node->sizeInBytes = nodeSize;
            new_main_memory_node->mainMemoryIdNode = main_memory_id_node;
            new_main_memory_node->fromPhysicalAddress = last_physical_address;

            if (current_main_memory_node)
                insert_new_memory_node(current_main_memory_node, new_main_memory_node,
                                       last_physical_address ? 0 : 1);//todo if before then replace root
            else {
                if (last_physical_address)
                    //insert after everything
                    insert_new_memory_node(last_main_memory_node, new_main_memory_node, 0);
                else
                    //the table is empty
                    _g_main_memory_table = new_main_memory_node;
            }

            current_main_memory_node = new_main_memory_node;
        }

        assert(current_main_memory_node != NULL);
        last_physical_address = current_main_memory_node->fromPhysicalAddress + current_main_memory_node->sizeInBytes;

        last_main_memory_node = current_main_memory_node;
        current_main_memory_node = current_main_memory_node->next;
    }
}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
    if (_g_main_memory_size - _g_used_memory_size < size_of_chunk) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;

        return NULL;
    }

    //create new main memory id node
    struct MainMemoryIdNode* main_memory_id_node = malloc(sizeof(struct MainMemoryIdNode));
    main_memory_id_node->sizeInBytes = size_of_chunk;
    main_memory_id_node->fromLinearAddressPointer = (char*)obtain_linear_address_for_chunk(size_of_chunk);

    if (_g_main_memory_ids_table)
        main_memory_id_node->previous = _g_main_memory_ids_table;

    _g_main_memory_ids_table = main_memory_id_node;

    allocate_chunk(size_of_chunk, main_memory_id_node);

    *error = M_ERR_OK;

    _g_used_memory_size += size_of_chunk;

    assert(main_memory_id_node != NULL);
    return main_memory_id_node->fromLinearAddressPointer;
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

        //free ids' nodes
        while (current_main_memory_id_node) {
            struct MainMemoryIdNode* next_main_memory_id_node = current_main_memory_id_node->previous;

            //free loaded into cache pages nodes
            struct CachedPage* current_cache_memory_node = current_main_memory_id_node->cachedPages;

            while (current_cache_memory_node) {
                struct CachedPage* next_cache_memory_node = current_cache_memory_node->next;

                free(current_cache_memory_node);

                current_cache_memory_node = next_cache_memory_node;
            }

            free(current_main_memory_id_node);

            current_main_memory_id_node = next_main_memory_id_node;
        }
    }

    _g_main_memory_size = number_of_pages * size_of_page;

    _g_main_memory = NULL;
    _g_main_memory_table = NULL;
    _g_main_memory_ids_table = NULL;
    _g_main_memory = malloc(_g_main_memory_size);
    _g_used_memory_size = 0;

    //set all available cache pages free
    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES; ++i)
        _g_cache_usage_table[i / sizeof(char) + i % sizeof(char)] = 0;
}
