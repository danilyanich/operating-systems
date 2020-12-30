#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "memory.h"


#define TRUE			1;
#define FALSE			0;

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 128;
int _g_bytes_allocated = 0;


struct block {
    struct block*	pNext;
    unsigned int	szBlock;
    void* data;
};

typedef struct page {
    struct block*	pFirstUse;
    unsigned int	maxSizeFreeBlock;
} page;

struct page *pageTable;

int numberOfPages = 0;
int sizePage = 0;

void addToUsedBlocks(struct block *_block, struct page *_page);
struct block* find_block(m_id ptr);

m_id m_malloc(int size_of_chunk, m_err_code* error) {
  int i;
  //Обход всех страниц
  for (i = 0; i < numberOfPages; i++) {
      //Проверка, хватает ли место в определенной странице
      if (pageTable[i].maxSizeFreeBlock >= size_of_chunk) {

              struct page *pagePtr = &pageTable[i];
              struct block *blockPtr = pagePtr->pFirstUse;

              if (blockPtr == NULL) {
                struct block* initialBlock = (struct block*) malloc(sizeof(struct block));
                initialBlock->pNext = NULL;
                initialBlock->szBlock = size_of_chunk;
                addToUsedBlocks(initialBlock, pagePtr);
                pagePtr->maxSizeFreeBlock -= size_of_chunk;
                m_id ptr = (m_id) initialBlock;
                *error = M_ERR_OK;
                return ptr;
              }

              while(blockPtr->pNext != NULL) {
                blockPtr = blockPtr->pNext;
              }

              struct block* block = (struct block*) malloc(sizeof(struct block));
              block->pNext = NULL;
              block->szBlock = size_of_chunk;
              addToUsedBlocks(block, pagePtr);
              pagePtr->maxSizeFreeBlock -= size_of_chunk;
              m_id ptr = (m_id) block;
              *error = M_ERR_OK;
              return ptr;
      }
  }
    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
}


void m_free(m_id ptr, m_err_code* error) {
    int i;
    for (i = 0; i < numberOfPages; i++) {
        struct page *pagePtr = &pageTable[i];
        struct block *usedBlockPtr = pagePtr->pFirstUse;
        struct block *parentBlock = NULL;

        if (usedBlockPtr == NULL) {
            continue;
        }

        while((m_id) usedBlockPtr != ptr && usedBlockPtr) {
            parentBlock = usedBlockPtr;
            usedBlockPtr = usedBlockPtr -> pNext;
        }

        if (usedBlockPtr == NULL) {
            continue;
        }

        if (parentBlock) {
            parentBlock->pNext = usedBlockPtr->pNext;
        } else {
            pagePtr->pFirstUse = usedBlockPtr->pNext;
        }

        pageTable[i].maxSizeFreeBlock += usedBlockPtr->szBlock;
        free(usedBlockPtr);
        *error = M_ERR_OK;
        return ;
    }
}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
    struct block* block = find_block(read_from_id);
    if (block == NULL) {
        printf("%s \n", "==M_READ== block is NULL");
        *error = M_ERR_INVALID_CHUNK;
        return ;
    }

    memcpy(read_to_buffer, block->data, size_to_read);
    printf("%s \n", "\n successful read from chunk");
    *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    struct block *block = find_block(write_to_id);
    printf("%s", " block size:");

    if (block != NULL) {
        printf("%i \n", block->szBlock);
    } else {
        printf("%s \n", "==M_WRITE== block is NULL");
        *error = M_ERR_INVALID_CHUNK;
        return ;
    }

    if (block->szBlock < size_to_write) {
        printf("%s \n", "==M_WRITE== Block size < size to write");
        *error = M_ERR_INVALID_CHUNK;
        return ;
    }

    void *data = malloc(size_to_write);
    memcpy(data, write_from_buffer, size_to_write);
    block->data = data;
    *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
    pageTable = (page*) malloc(sizeof(page) * number_of_pages);
    int numberOfRAMPages = 0;
    numberOfPages = number_of_pages;
    for (int i = 0; i < number_of_pages; i++) {
        //инициализация страницы
        pageTable[i].pFirstUse = NULL;
        pageTable[i].maxSizeFreeBlock = size_of_page;
    }
}

/**
 	@func	addToUsedBlocks
 	@brief	Добавляет блок в список занятых блоков указанной страницы
	@param	[in] _block			указатель на блок
	@param	[in] _page			указатель на страницу
 **/
void addToUsedBlocks(struct block *_block, struct page *_page)
{
    struct block *usedBlockPtr = _page -> pFirstUse;
    struct block *parentBlockPtr = NULL;

    while (usedBlockPtr)
    {
        parentBlockPtr = usedBlockPtr;
        usedBlockPtr = usedBlockPtr -> pNext;
    }
    if (parentBlockPtr)
    {
        parentBlockPtr -> pNext = _block;
    }
    else
    {
        _page -> pFirstUse = _block;
    }
}


/**
 	@func	find_block
 	@brief	Находит блок, заданного указателя, в таблице
	@param	[in] _ptr			указатель на блока
 **/
struct block* find_block(m_id ptr) {
    int i;
    printf("\n");
    for (i = 0; i < numberOfPages; i++) {
        printf("%s", "number of page: ");
        printf("%i \n", i);
        struct page *pagePtr = &pageTable[i];
        struct block *usedBlockPtr = pagePtr->pFirstUse;

        if (usedBlockPtr == NULL) {
            continue;
        }
        while((m_id) usedBlockPtr != ptr && usedBlockPtr) {
            usedBlockPtr = usedBlockPtr -> pNext;
        }

        if (usedBlockPtr == NULL) {
            continue;
        }

        return usedBlockPtr;
    }
    return NULL;
}