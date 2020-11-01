#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include "memory.h"

#define CACHE_SIZE_IN_PAGES 8
#define LOCALITY_SIZE 2
#define PAGE_SIZE_IN_BYTES 16

struct Node {
    struct Node* previous;
    struct Node* next;
};

struct MainMemoryIdNode {
    struct MainMemoryIdNode* next;
    struct MainMemoryIdNode* previous;
    char* fromVirtualAddressPointer;
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
    struct CachedPage* previous;
    struct CachedPage* next;
    unsigned cachePageNumber; //number of page in cache
    unsigned pageNumber; //number of page of the segment
};

struct CachePage {
    struct MainMemoryIdNode* id;
    struct CachedPage* cachedPage;
    unsigned char flags; //XXXXXXXC, X - reserved for further standardization)), C - a flag which indicates whether the page was changed or not
};

struct MainMemoryIdNode* _g_main_memory_ids_table;

struct CachePage _g_cache_table[CACHE_SIZE_IN_PAGES];
struct MainMemoryNode* _g_main_memory_table = NULL;
char _g_cache_memory[CACHE_SIZE_IN_PAGES * PAGE_SIZE_IN_BYTES];
char* _g_main_memory = NULL;

unsigned long _g_main_memory_size;
unsigned long _g_used_memory_size;

void remove_node(struct Node* node, struct Node** root) {
    if (node->previous) {
        node->previous->next = node->next;

        if (node->next)
            node->next->previous = node->previous;
    } else {
        *root = node->next;

        if (node->next)
            node->next->previous = NULL;
    }
}

void insert_new_node(struct Node* node_anchor,
                     struct Node* node_to_insert, char insert_after_anchor, struct Node** root) {
    if (insert_after_anchor) {
        if (node_anchor->next)
            insert_new_node(node_anchor->next, node_to_insert, 0, root);
        else {
            //after the last one
            node_anchor->next = node_to_insert;

            node_to_insert->previous = node_anchor;
            node_to_insert->next = NULL;
        }
    } else {
        //before
        struct Node* another_side = node_anchor->previous;

        node_anchor->previous = node_to_insert;

        node_to_insert->previous = another_side;
        node_to_insert->next = node_anchor;

        if (another_side)
            another_side->next = node_to_insert;
        else
            *root = node_to_insert;
    }
}

void write_directly(char* to, char* from, unsigned size) {
    bcopy(from, to, size);
}

void write_to_main_memory(struct MainMemoryIdNode* id, unsigned address, unsigned size, char* from) {
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    unsigned memory_to_update = size;
    unsigned last_virtual_address = 0;

    while (memory_to_update) {
        if (current_main_memory_node->mainMemoryIdNode == id) {
            unsigned from_virtual_address = last_virtual_address;
            unsigned to_virtual_address = last_virtual_address + current_main_memory_node->sizeInBytes;

            if ((address >= from_virtual_address && address <= to_virtual_address) || memory_to_update != size) {
                unsigned address_stub = memory_to_update != size ? 0 : address;

                unsigned available = to_virtual_address - from_virtual_address - address_stub;
                unsigned write_size = memory_to_update > available ? available : memory_to_update;

                write_directly(_g_main_memory + current_main_memory_node->fromPhysicalAddress + address_stub,
                        from + (size - memory_to_update), write_size);

                memory_to_update -= write_size;
            }

            last_virtual_address = to_virtual_address;
        }

        current_main_memory_node = current_main_memory_node->next;
    }
}

//makes cache page free
void free_cache_page(unsigned cache_page_number) {
    _g_cache_table[cache_page_number] = (struct CachePage){NULL, 0, 0};
}

//removes (and deallocates) cached page node from id node, frees cache page
void free_cached_page(struct MainMemoryIdNode* main_memory_id_node, struct CachedPage* cached_page) {
    remove_node(cached_page, &main_memory_id_node->cachedPages);

    free_cache_page(cached_page->cachePageNumber);
    free(cached_page);
}

void unload_cache_page(unsigned cache_page_number) {
    struct CachePage* cache_page = _g_cache_table + cache_page_number;
    struct MainMemoryIdNode* id = cache_page->id;

    if (id && (cache_page->flags & 0b1u)) {
        write_to_main_memory(id, cache_page_number * PAGE_SIZE_IN_BYTES, PAGE_SIZE_IN_BYTES,
                _g_cache_memory + cache_page_number * PAGE_SIZE_IN_BYTES);
        free_cached_page(id, cache_page->cachedPage);
    }
}

void load_context_into_cache(unsigned address, unsigned size) {
    //determine id
    struct MainMemoryIdNode* id = NULL;
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

    while (current_main_memory_id_node) {
        unsigned from_virtual_address = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

        if (address >= from_virtual_address
            && address <= from_virtual_address + current_main_memory_id_node->sizeInBytes) {
            id = current_main_memory_id_node;

            break;
        }

        current_main_memory_id_node = current_main_memory_id_node->next;
    }

    assert(id != NULL);

    //determine and load required pages (not more than cache_size at once)
    //when unloading cache pages, do it properly
}

void free_main_memory_node(struct MainMemoryNode* main_memory_node) {
    remove_node(main_memory_node, &_g_main_memory_table);

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

        current_main_memory_node = current_main_memory_node->next;
    }

    //main memory id node
    remove_node(main_memory_id_node, &_g_main_memory_ids_table);
}

unsigned calculate_to_address(struct MainMemoryIdNode* main_memory_id_node) {
    return main_memory_id_node->fromVirtualAddressPointer - (char*)0 + main_memory_id_node->sizeInBytes;
}

struct MainMemoryIdNode* obtain_id_node_for_new_node_to_be_inserted_after(unsigned size_of_chunk) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

    if (!current_main_memory_id_node)
        return NULL;
    else {
        struct MainMemoryIdNode* maximal_address_node = current_main_memory_id_node;
        unsigned last_from = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

        while (current_main_memory_id_node) {
            unsigned current_to = calculate_to_address(current_main_memory_id_node);
            long gap = (long)last_from - current_to;

            if (gap > size_of_chunk)
                return current_main_memory_id_node;

            current_main_memory_id_node = current_main_memory_id_node->previous;
        }

        return maximal_address_node;
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

        char autoFollow = 0;

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

            if (current_main_memory_node) {
                insert_new_node(current_main_memory_node, new_main_memory_node, 0, &_g_main_memory_table);

                autoFollow = 1;
            } else {
                if (last_physical_address)
                    //insert after everything
                    insert_new_node(last_main_memory_node, new_main_memory_node, 1, &_g_main_memory_table);
                else
                    //the table is empty
                    _g_main_memory_table = new_main_memory_node;
            }

            if (!autoFollow)
                current_main_memory_node = new_main_memory_node;
        } else
            autoFollow = 1;

        assert(current_main_memory_node != NULL);
        last_physical_address = current_main_memory_node->fromPhysicalAddress + current_main_memory_node->sizeInBytes;

        last_main_memory_node = current_main_memory_node;

        if (autoFollow)
            current_main_memory_node = current_main_memory_node->next;
    }
}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
    if (_g_main_memory_size - _g_used_memory_size < size_of_chunk) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;

        return NULL;
    }

    //create new main memory id node
    struct MainMemoryIdNode* id_node_to_insert_after = obtain_id_node_for_new_node_to_be_inserted_after(size_of_chunk);

    struct MainMemoryIdNode* main_memory_id_node = malloc(sizeof(struct MainMemoryIdNode));
    main_memory_id_node->sizeInBytes = size_of_chunk;

    if (id_node_to_insert_after) {
        insert_new_node(id_node_to_insert_after, main_memory_id_node, 0, &_g_main_memory_ids_table);

        main_memory_id_node->fromVirtualAddressPointer = id_node_to_insert_after->fromVirtualAddressPointer
                                                         + id_node_to_insert_after->sizeInBytes;
    } else {
        _g_main_memory_ids_table = main_memory_id_node;

        main_memory_id_node->fromVirtualAddressPointer = 0;
    }

    allocate_chunk(size_of_chunk, main_memory_id_node);

    *error = M_ERR_OK;

    _g_used_memory_size += size_of_chunk;

    assert(main_memory_id_node != NULL);
    return main_memory_id_node->fromVirtualAddressPointer;
}

//if a chunk already was deallocated or even never existed, then E_INVALID_CHUNK will be thrown
void m_free(m_id ptr, m_err_code* error) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

    while (current_main_memory_id_node) {
        if (current_main_memory_id_node->fromVirtualAddressPointer == (char*)ptr) {
            free_segment(current_main_memory_id_node);

            _g_used_memory_size -= current_main_memory_id_node->sizeInBytes;
            *error = M_ERR_OK;

            return;
        }

        current_main_memory_id_node = current_main_memory_id_node->previous;
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

    //unnecessary
    for (unsigned i = 0; i < _g_main_memory_size; ++i)
        _g_main_memory[i] = 0;

    _g_used_memory_size = 0;

    //set all available cache pages free
    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES; ++i)
        free_cache_page(i);
}

void dump() {
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    printf("--MEMORY DUMP BEGINNING--\n");

    while (current_main_memory_node) {
        printf("VRTF: %d, MMSZ: %d, SIZE: %d, PHSF: %d\n",
               (unsigned)(current_main_memory_node->mainMemoryIdNode->fromVirtualAddressPointer - (char*)0),
               current_main_memory_node->mainMemoryIdNode->sizeInBytes, current_main_memory_node->sizeInBytes,
               current_main_memory_node->fromPhysicalAddress);

        printf("DATA: ");

        for (unsigned i = 0; i < current_main_memory_node->sizeInBytes; ++i)
            printf("%d", *(_g_main_memory + i));

        printf("\n");

        current_main_memory_node = current_main_memory_node->next;
    }

    printf("--MEMORY DUMP END--\n\n");
}