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

void make_one_page_transit_with_main_memory(struct MainMemoryIdNode* id, unsigned virtual_address, unsigned size, char* buffer, char read_or_write) {
    //0 - read, reads `size` bytes beginning from `virtual_address` into buffer
    //1 - write, writes `size` bytes from `buffer` to `virtual_address`
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    unsigned bytes_to_transit = size;
    unsigned last_virtual_address = 0;

    while (bytes_to_transit) {
        if (current_main_memory_node->mainMemoryIdNode == id) {
            unsigned from_virtual_address = last_virtual_address;
            unsigned to_virtual_address = last_virtual_address + current_main_memory_node->sizeInBytes;

            if ((virtual_address >= from_virtual_address && virtual_address <= to_virtual_address) || bytes_to_transit != size) {
                unsigned address_stub = bytes_to_transit != size ? 0 : virtual_address;

                unsigned available = to_virtual_address - from_virtual_address - address_stub;
                unsigned bytes_to_transit_at_once = bytes_to_transit > available ? available : bytes_to_transit;

                char* buffer_ptr = buffer + (size - bytes_to_transit);
                char* memory_ptr = _g_main_memory + current_main_memory_node->fromPhysicalAddress + address_stub;

                if (read_or_write) //write to mem
                    bcopy(buffer_ptr, memory_ptr, bytes_to_transit_at_once);
                else //read to buffer
                    bcopy(memory_ptr, buffer_ptr, bytes_to_transit_at_once);

                bytes_to_transit -= bytes_to_transit_at_once;
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
        make_one_page_transit_with_main_memory(id, cache_page_number * PAGE_SIZE_IN_BYTES, PAGE_SIZE_IN_BYTES,
                                               _g_cache_memory + cache_page_number * PAGE_SIZE_IN_BYTES, 1);
        free_cached_page(id, cache_page->cachedPage);
    }
}

struct MainMemoryIdNode* determine_id_of_address(unsigned address) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

    while (current_main_memory_id_node) {
        unsigned from_virtual_address = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

        if (address >= from_virtual_address
            && address <= from_virtual_address + current_main_memory_id_node->sizeInBytes)
            return current_main_memory_id_node;

        current_main_memory_id_node = current_main_memory_id_node->next;
    }

    return NULL;
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

//returns from-pointer
char* place_id_node(struct MainMemoryIdNode* main_memory_id_node) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;
    unsigned size_of_chunk = main_memory_id_node->sizeInBytes;

    if (!current_main_memory_id_node) {
        //empty table
        _g_main_memory_ids_table = main_memory_id_node;

        return 0;
    } else {
        struct MainMemoryIdNode* maximal_address_node = current_main_memory_id_node;
        unsigned last_from = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

        current_main_memory_id_node = current_main_memory_id_node->previous;

        while (current_main_memory_id_node) {
            unsigned current_to = calculate_to_address(current_main_memory_id_node);
            long gap = (long)last_from - current_to;

            if (gap >= size_of_chunk) {
                //after some node
                insert_new_node(current_main_memory_id_node, main_memory_id_node, 0, &_g_main_memory_ids_table);
                return (char*)calculate_to_address(current_main_memory_id_node);
            }

            last_from = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

            if (!current_main_memory_id_node->previous && last_from >= size_of_chunk) {
                //before everything
                main_memory_id_node->next = current_main_memory_id_node;
                main_memory_id_node->previous = NULL;

                current_main_memory_id_node->previous = main_memory_id_node;

                return 0;
            }

            current_main_memory_id_node = current_main_memory_id_node->previous;
        }

        //after everything
        insert_new_node(maximal_address_node, main_memory_id_node, 0, &_g_main_memory_ids_table);
        return (char*)calculate_to_address(maximal_address_node);
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

struct CachePage* provide_cache_page(struct MainMemoryIdNode* main_memory_id_node, unsigned page_of_segment) {

}

void make_transit_with_main_memory(unsigned virtual_address, unsigned size, char* buffer, char read_or_write, m_err_code* error) {
    //0 - read
    //1 - write
    struct MainMemoryIdNode* id = determine_id_of_address(virtual_address);

    if (id) {
        unsigned local_address = virtual_address - (id->fromVirtualAddressPointer - (char*)0);
        unsigned first_affected_page = local_address / PAGE_SIZE_IN_BYTES;
        unsigned last_affected_page = (local_address + size - 1) / PAGE_SIZE_IN_BYTES;
        unsigned pages_remain = id->sizeInBytes / PAGE_SIZE_IN_BYTES - last_affected_page - 1;
        unsigned locality_pages_count = pages_remain > LOCALITY_SIZE ? LOCALITY_SIZE : pages_remain;

        unsigned bytes_to_transit = size;
        unsigned offset = local_address - first_affected_page * PAGE_SIZE_IN_BYTES;

        for (unsigned p = first_affected_page; p <= last_affected_page + locality_pages_count; ++p) {
            struct CachePage* page = provide_cache_page(id, p);

            if (p <= last_affected_page) {
                unsigned available = PAGE_SIZE_IN_BYTES - offset;
                unsigned bytes_to_transit_at_once = available > bytes_to_transit ? bytes_to_transit : available;

                char *buffer_ptr = buffer + (size - bytes_to_transit);
                char *cache_ptr = _g_cache_memory + page->cachedPage->cachePageNumber * PAGE_SIZE_IN_BYTES + offset;

                if (read_or_write)
                    bcopy(buffer_ptr, cache_ptr, bytes_to_transit_at_once);
                else
                    bcopy(cache_ptr, buffer_ptr, bytes_to_transit_at_once);

                bytes_to_transit -= bytes_to_transit_at_once;
            }
        }
    } else {
        *error = M_ERR_OUT_OF_BOUNDS;

        return;
    }

    *error = M_ERR_OK;
}

m_id m_malloc(int size_of_chunk, m_err_code* error) {
    if (_g_main_memory_size - _g_used_memory_size < size_of_chunk) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;

        return NULL;
    }

    //create new main memory id node
    struct MainMemoryIdNode* main_memory_id_node = malloc(sizeof(struct MainMemoryIdNode));

    main_memory_id_node->sizeInBytes = size_of_chunk;
    main_memory_id_node->fromVirtualAddressPointer = place_id_node(main_memory_id_node);

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
    make_transit_with_main_memory(read_from_id - (void*)0, size_to_read, read_to_buffer, 0, error);
}

void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    make_transit_with_main_memory(write_to_id - (void*)0, size_to_write, write_from_buffer, 1, error);
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
    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES * PAGE_SIZE_IN_BYTES; ++i)
        _g_cache_memory[i] = 0;
    //end of unnecessary

    _g_used_memory_size = 0;

    //set all available cache pages free
    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES; ++i)
        free_cache_page(i);
}

void dump() {
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    printf("--MEMORY DUMP BEGINNING--\n");

    while (current_main_memory_node) {
        printf("MMID: %p, VRTF: %d, MMSZ: %d, SIZE: %d, PHSF: %d\n",
               current_main_memory_node->mainMemoryIdNode,
               (unsigned)(current_main_memory_node->mainMemoryIdNode->fromVirtualAddressPointer - (char*)0),
               current_main_memory_node->mainMemoryIdNode->sizeInBytes, current_main_memory_node->sizeInBytes,
               current_main_memory_node->fromPhysicalAddress);

        printf("DATA: ");

        for (unsigned i = 0; i < current_main_memory_node->sizeInBytes; ++i)
            printf("%d", *(_g_main_memory + current_main_memory_node->fromPhysicalAddress + i));

        printf("\n");

        current_main_memory_node = current_main_memory_node->next;
    }

    printf("\nCACHE:\n");

    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES; ++i) {
        struct CachePage* cache_page = _g_cache_table + i;

        printf("MMID: %p, PAGE: %d, MMPG: %d, FLGS: %#x\n", cache_page->id, i,
               cache_page->cachedPage ? cache_page->cachedPage->pageNumber : 0, cache_page->flags);

        printf("DATA: ");

        for (unsigned j = 0; j < PAGE_SIZE_IN_BYTES; ++j)
            printf("%d", *(_g_cache_memory + (PAGE_SIZE_IN_BYTES * i) + j));

        printf("\n");
    }

    printf("--MEMORY DUMP END--\n\n");
}