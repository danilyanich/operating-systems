#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "memory.h"

#define CACHE_SIZE_IN_PAGES 8
#define LOCALITY_SIZE 2
#define PAGE_SIZE_IN_BYTES 16

struct MainMemoryIdNode {
    struct MainMemoryIdNode* previous;
    struct MainMemoryIdNode* next;
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

void free_cache_page(unsigned cache_page_number) {
    _g_cache_usage_table[cache_page_number / sizeof(char) + cache_page_number % sizeof(char)] = 0;
}

void free_cached_page(struct MainMemoryIdNode* main_memory_id_node, struct CachedPage* cached_page) {
    if (main_memory_id_node->cachedPages == cached_page)
        main_memory_id_node->cachedPages = cached_page->next;

    free_cache_page(cached_page->cachePageNumber);
    free(cached_page);
}

void free_main_memory_node(struct MainMemoryNode* main_memory_node) {
    if (main_memory_node->previous) {
        main_memory_node->previous->next = main_memory_node->next;

        if (main_memory_node->next)
            main_memory_node->next->previous = main_memory_node->previous;
    } else {
        _g_main_memory_table = main_memory_node->next;

        if (main_memory_node->next)
            main_memory_node->next->previous = NULL;
    }

    free(main_memory_node);
}

void free_segment(struct MainMemoryIdNode* main_memory_id_node) {
    //cache + cache pages
    struct CachedPage* current_cached_page = main_memory_id_node->cachedPages;

    while (current_cached_page) {
        struct CachedPage* next_cached_page = current_cached_page->next;

        free_cached_page(main_memory_id_node, current_cached_page);

        current_cached_page = next_cached_page;
    }

    //main memory nodes
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    while (current_main_memory_node) {
        if (current_main_memory_node->mainMemoryIdNode == main_memory_id_node)
            free_main_memory_node(current_main_memory_node);
    }

    //main memory id node
    if (main_memory_id_node->previous) {
        main_memory_id_node->previous->next = main_memory_id_node->next;

        if (main_memory_id_node->next)
            main_memory_id_node->next->previous = main_memory_id_node->previous;
    } else {
        _g_main_memory_ids_table = main_memory_id_node->next;

        if (main_memory_id_node->next)
            main_memory_id_node->next->previous = NULL;
    }
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
            new_main_memory_node->next = NULL;
            new_main_memory_node->previous = NULL;

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
    main_memory_id_node->previous = NULL;
    main_memory_id_node->next = NULL;

    if (_g_main_memory_ids_table) {
        main_memory_id_node->previous = _g_main_memory_ids_table;
        _g_main_memory_ids_table->next = main_memory_id_node;
    }

    _g_main_memory_ids_table = main_memory_id_node;

    allocate_chunk(size_of_chunk, main_memory_id_node);

    *error = M_ERR_OK;

    _g_used_memory_size += size_of_chunk;

    assert(main_memory_id_node != NULL);
    return main_memory_id_node->fromLinearAddressPointer;
}

//if a chunk already was deallocated or even never existed, then E_INVALID_CHUNK will be thrown
void m_free(m_id ptr, m_err_code* error) {
    struct MainMemoryIdNode* current_main_memory_node = _g_main_memory_ids_table;

    while (current_main_memory_node) {
        if (current_main_memory_node->fromLinearAddressPointer == ptr) {
            free_segment(current_main_memory_node);

            *error = M_ERR_OK;

            return;
        }

        current_main_memory_node = current_main_memory_node->next;
    }

    *error = M_ERR_INVALID_CHUNK;
}

void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {

}

void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {

}

void m_init(int number_of_pages, int size_of_page) {
    if (_g_main_memory) {
        //free the main memory
        free(_g_main_memory);

        //free ids' nodes
        while (_g_main_memory_ids_table) {
            free_segment(_g_main_memory_ids_table);
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
        free_cache_page(i);
}