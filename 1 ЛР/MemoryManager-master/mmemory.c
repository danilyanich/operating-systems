#include "mmemory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#define TRUE			1
#define FALSE			0
#define ALLOCSIZE		128				
#define NULL_SYMBOL		'\0'			
#define SWAPING_NAME	"swap.dat"		

/* Коды возврата */
const int SUCCESSFUL_CODE				=  0;
const int INCORRECT_PARAMETERS_ERROR	= -1;
const int NOT_ENOUGH_MEMORY_ERROR		= -2;
const int OUT_OF_RANGE_ERROR			= -2;
const int UNKNOWN_ERROR					=  1;

/* Блок непрерывной памяти (занятой или свободный) */
struct block {
	struct block*	pNext;			
	unsigned int	szBlock;			
	unsigned int	offsetBlock;	
};

/* Информация о странице в таблице страниц */
struct pageInfo {
	unsigned long	offsetPage;		
	char			isUse;			
};

/* Заголовок страницы */
typedef struct page {
	struct block*	pFirstFree;			
	struct block*	pFirstUse;			
	unsigned int	maxSizeFreeBlock;		
	struct pageInfo	pInfo;				
	unsigned int	activityFactor;		
} page;

/* Таблица страниц */
struct page *pageTable; 

/* Количество страниц (в памяти и на диске) */
int numberOfPages = 0;

/* Размер страницы */
int sizePage = 0;

/* Физическая память */
VA allocbuf;

void ___print_memory();
int initPageTable();
void addToUsedBlocks(struct block *_block, struct page *_page);
void addToFreeBlocks(struct block *_block, struct page *_page);
int mallocInPage(VA* ptr, size_t szBlock, int pageNum);
void nullMemory();
unsigned int getMaxSizeFreeBlock(struct page _page);
struct page* getLeastActivePageInMemory();
int swap(struct page *memoryPage, struct page *swopPage);
int readf(unsigned long offsetPage, VA readPage);
int writef(unsigned long offsetPage, VA writtenPage);
VA convertVirtualtoPhysicalAddress(VA ptr, int *offsetPage, int *offsetBlock);
struct block* findBlock(int offsetPage, int offsetBlock);
struct block* findParentBlock(int offsetPage, int offsetBlock);

int _malloc (VA* ptr, size_t szBlock)
{
	int i;
	struct page *reservePageInSwap = NULL;
	int reservePageNum;

	if (!ptr || szBlock > (unsigned int) sizePage)
	{
		return INCORRECT_PARAMETERS_ERROR;
	}
	
	for (i = 0; i < numberOfPages; i++)
	{
		
		if (pageTable[i].maxSizeFreeBlock >= szBlock)
		{
			
			if (pageTable[i].pInfo.isUse)
			{
				return mallocInPage(ptr, szBlock, i);
			}
			else if (!reservePageInSwap)
			{
				reservePageInSwap = &pageTable[i];
				reservePageNum = i;
			}
		}
	}
	
	if (reservePageInSwap)
	{
		int __errno = swap(getLeastActivePageInMemory(), reservePageInSwap);
		return mallocInPage(ptr, szBlock, reservePageNum);
	}
	return NOT_ENOUGH_MEMORY_ERROR;
}

int _free (VA ptr)
{
	VA raddr;
	int indexPage, offsetBlock;
	struct block *freeBlock, *parentBlock;
	if (!ptr)
	{
		return INCORRECT_PARAMETERS_ERROR;
	}
	raddr = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);
	freeBlock = findBlock(indexPage, offsetBlock);
	parentBlock = findParentBlock(indexPage, offsetBlock);
	if (parentBlock)
	{
		parentBlock -> pNext = freeBlock -> pNext;
	}
	else
	{
		pageTable[indexPage].pFirstUse = freeBlock -> pNext;
	}
	addToFreeBlocks(freeBlock, &pageTable[indexPage]);
	return SUCCESSFUL_CODE;
}

int _read (VA ptr, void* pBuffer, size_t szBuffer)
{
	VA raddr, vaBuffer;
	int indexPage, offsetBlock;
	unsigned int j;
	struct block* readBlock;
	if (!ptr || !pBuffer || (szBuffer > (unsigned int) sizePage))
	{
		return INCORRECT_PARAMETERS_ERROR;
	}
	raddr = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);

	if (!raddr)
	{
		int __errno = swap(getLeastActivePageInMemory(), &pageTable[indexPage]);
		raddr = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);
	}
	
	readBlock = findBlock(indexPage, offsetBlock);
	if (szBuffer <= readBlock -> szBlock)
	{
		VA raddr2;
		vaBuffer = (VA) pBuffer;
		raddr2 = convertVirtualtoPhysicalAddress(vaBuffer, &indexPage, &offsetBlock);

		if (!raddr2)
		{
			int __errno = swap(getLeastActivePageInMemory(), &pageTable[indexPage]);
			raddr2 = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);
		}

		for (j = 0; j < szBuffer; j++)
			raddr2[j] = raddr[j];
		pBuffer = (void*) vaBuffer;
		return SUCCESSFUL_CODE;
	}
	
	else
	{
		return OUT_OF_RANGE_ERROR;
	}	
}

int _write (VA ptr, void* pBuffer, size_t szBuffer)
{
	VA raddr, vaBuffer;
	int indexPage, offsetBlock;
	struct block* readBlock;
	unsigned int j;
	if (!ptr || !pBuffer || (szBuffer > (unsigned int) sizePage))
	{
		return INCORRECT_PARAMETERS_ERROR;
	}
	raddr = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);
	if (!raddr)
	{
		int __errno = swap(getLeastActivePageInMemory(), &pageTable[indexPage]);
		raddr = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);
	}
	readBlock = findBlock(indexPage, offsetBlock);
	if (szBuffer <= readBlock -> szBlock)
	{
		vaBuffer = (VA) pBuffer;
		for (j = 0; j < szBuffer; j++)
			raddr[j] = vaBuffer[j];
		pageTable[indexPage].activityFactor++;
		return SUCCESSFUL_CODE;
	}
	
	else
	{
		return OUT_OF_RANGE_ERROR;
	}
	
}

int _init (int n, int szPage)
{	
	remove(SWAPING_NAME);
	if (n > 0 && szPage > 0)
	{
		sizePage = szPage;
		numberOfPages = n;
		pageTable = (page*) malloc(sizeof(page) * n);
		allocbuf = (VA) malloc(sizeof(char) * ALLOCSIZE);
		nullMemory();
		if (allocbuf && pageTable)
		{						
			return initPageTable();
		}
		else
		{			
			return UNKNOWN_ERROR;
		}
	}
	return INCORRECT_PARAMETERS_ERROR;
}

int initPageTable()
{
	int numberOfSwapPages = 0;
	int numberOfRAMPages = 0;
	int __errno;
	int i;

	for (i = 0; i < numberOfPages; i++)
	{
		struct block* firstBlock = (struct block *) malloc(sizeof(struct block));
		firstBlock -> pNext = NULL;
		firstBlock -> szBlock = sizePage;
		firstBlock -> offsetBlock = 0;

		pageTable[i].pFirstFree = firstBlock;
		pageTable[i].pFirstUse = NULL;
		pageTable[i].maxSizeFreeBlock = sizePage;				
		pageTable[i].activityFactor = 0;

		if ((i+1) * sizePage <= ALLOCSIZE)
		{			
			pageTable[i].pInfo.offsetPage = numberOfRAMPages++;
			pageTable[i].pInfo.isUse = TRUE;
		}
		else
		{
			__errno = writef(numberOfSwapPages, allocbuf);
			if (!__errno)
			{
				pageTable[i].pInfo.offsetPage = numberOfSwapPages++;
				pageTable[i].pInfo.isUse = FALSE;
			}
			else
			{
				return UNKNOWN_ERROR;
			}
		}
	}
	return SUCCESSFUL_CODE;
}

void ___print_memory()
{
	int i;
	for (i = 0; i < ALLOCSIZE; i++)
	{
		printf("%c", allocbuf[i]);
	}
	printf("%c", '\n');
}

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

void addToFreeBlocks(struct block *_block, struct page *_page)
{
	struct block *freeBlockPtr = _page -> pFirstFree;
	struct block *parentBlockPtr = NULL;

	while (freeBlockPtr)
	{
		parentBlockPtr = freeBlockPtr;
		freeBlockPtr = freeBlockPtr -> pNext;
	}
	
	if (parentBlockPtr)
	{
		if ((parentBlockPtr -> offsetBlock + parentBlockPtr -> szBlock)
			== _block -> offsetBlock)
		{
			parentBlockPtr -> szBlock += _block -> szBlock;
			free(freeBlockPtr);
		}
		else
		{
			parentBlockPtr -> pNext = _block;
			_block -> pNext = NULL;
		}
	}
	else
	{
		_page -> pFirstFree = _block;
	}
}

int mallocInPage(VA* ptr, size_t szBlock, int pageNum)
{
	struct page *_page = &pageTable[pageNum];
	struct block *blockPtr = _page -> pFirstFree;
	struct block *parentBlockPtr = NULL;
	while (blockPtr)
	{
		if (blockPtr -> szBlock >= szBlock)
		{
			int vaddr = pageNum * sizePage + blockPtr -> offsetBlock + 1;
			*ptr = (VA) (vaddr);	
			if (blockPtr -> szBlock == szBlock)
			{
				if (parentBlockPtr)
					parentBlockPtr -> pNext = blockPtr -> pNext;
				else
					_page -> pFirstFree = NULL;
				blockPtr -> pNext = NULL;
				addToUsedBlocks(blockPtr, _page);
			}
			else
			{
				struct block* usedBlock;
				usedBlock = (struct block *) malloc(sizeof(struct block));
				usedBlock -> offsetBlock = blockPtr -> offsetBlock;
				usedBlock -> pNext = NULL;
				usedBlock -> szBlock = szBlock;
				addToUsedBlocks(usedBlock, _page);
				blockPtr -> offsetBlock += szBlock;
				blockPtr -> szBlock -= szBlock;				
			}
			_page -> maxSizeFreeBlock = getMaxSizeFreeBlock(*_page);
			_page -> activityFactor++;
			return SUCCESSFUL_CODE;
		}
		else
		{
			parentBlockPtr = blockPtr;
			blockPtr = blockPtr -> pNext;
		}
	}
	return UNKNOWN_ERROR;
}


void nullMemory()
{
	int i;
	for (i = 0; i < ALLOCSIZE; i++)
	{
		allocbuf[i] = NULL_SYMBOL;
	}
}

unsigned int getMaxSizeFreeBlock(struct page _page)
{
	unsigned int maxSize = 0;
	struct block *blockPtr = _page.pFirstFree;
	while (blockPtr)
	{
		if (blockPtr -> szBlock > maxSize)
		{
			maxSize = blockPtr -> szBlock;
		}
		blockPtr = blockPtr -> pNext;
	}
	return maxSize;
}

struct page* getLeastActivePageInMemory()
{	
	int i;	
	int minActivityIndex;
	struct page* minActivityPage = NULL;
	for (i = 0; i < numberOfPages; i++)
	{
		if (pageTable[i].pInfo.isUse)
		{
			if (!minActivityPage ||
				pageTable[i].activityFactor < minActivityPage -> activityFactor)
			{
				minActivityPage = &pageTable[i];
				minActivityIndex = i;
			}
		}
	}
	return minActivityPage;
}

VA convertVirtualtoPhysicalAddress(VA ptr, int *offsetPage, int *offsetBlock)
{
	struct block *trueBlock, *tempBlock;
	int maxBlockOffset;
	const int SIGN_DIFFERENCE = 256;
	int vaddr = (int)ptr - 1;
	VA allocp = NULL;

	if (vaddr < 0)
	{
		vaddr = SIGN_DIFFERENCE * sizeof(char) + vaddr; 
	}

	*offsetPage = vaddr / sizePage;
	*offsetBlock = vaddr % sizePage;
		
	if (*offsetPage <= numberOfPages && pageTable[*offsetPage].pInfo.isUse)
	{
		allocp = allocbuf + pageTable[*offsetPage].pInfo.offsetPage * sizePage + *offsetBlock;
	}

	tempBlock = pageTable[*offsetPage].pFirstUse;
	maxBlockOffset = tempBlock -> offsetBlock;
	while (tempBlock && tempBlock -> pNext && tempBlock -> pNext -> offsetBlock <= *offsetBlock)
		tempBlock = tempBlock -> pNext;
	*offsetBlock = tempBlock -> offsetBlock;
	return allocp;
}

struct block* findBlock(int offsetPage, int offsetBlock)
{
	struct block* blockPtr = pageTable[offsetPage].pFirstUse;
	while (blockPtr && blockPtr -> offsetBlock != offsetBlock)
	{
		blockPtr = blockPtr -> pNext;
	}
	return blockPtr;
}

struct block* findParentBlock(int offsetPage, int offsetBlock)
{
	struct block* parentBlock = NULL;
	struct block* blockPtr = pageTable[offsetPage].pFirstUse;
	while (blockPtr && blockPtr -> offsetBlock != offsetBlock)
	{
		parentBlock = blockPtr;
		blockPtr = blockPtr -> pNext;
	}
	return parentBlock;
}

int swap(struct page *memoryPage, struct page *swopPage)
{
	const int add_offset = 2;
	struct pageInfo bufInfo;
	int __errno, j;

	VA memoryPtr = allocbuf + memoryPage -> pInfo.offsetPage * sizePage - add_offset;
	VA memoryPageContent = (VA) malloc(sizeof(char) * sizePage);

	for (j = 0; j < sizePage; j++)
		memoryPageContent[j] = memoryPtr[j];
	__errno = readf(swopPage -> pInfo.offsetPage, memoryPtr);
	if (!__errno)
		__errno = writef(swopPage -> pInfo.offsetPage, memoryPageContent);

	bufInfo = memoryPage -> pInfo;
	memoryPage -> pInfo = swopPage -> pInfo;
	swopPage -> pInfo = bufInfo;
	memoryPage -> activityFactor++;
	swopPage -> activityFactor++;
	return __errno;
}

int readf(unsigned long offsetPage, VA readPage)
{
	const int FILE_NOT_FOUND = -1;
	const int UNEXPECTED_EOF = -2;

	FILE *fp;
	int returnValue;
	long fileOffset;
	int __errno;

	fileOffset = offsetPage * sizePage;
	__errno = fopen_s(&fp, SWAPING_NAME, "r");
	if (!__errno)
	{
		__errno = fseek(fp, fileOffset, SEEK_SET);
		if (!__errno)
		{
			
			int i;
			for (i = 0; i < sizePage; i++)
				readPage[i] = fgetc(fp);
			returnValue = SUCCESSFUL_CODE;
		}
		else
		{
			returnValue = UNEXPECTED_EOF;
		}
		fclose(fp);
	}
	else
	{
		returnValue = FILE_NOT_FOUND;
	}
	return returnValue;
}

int writef(unsigned long offsetPage, VA writtenPage)
{
	const int FILE_NOT_FOUND = -1;
	const int UNEXPECTED_EOF = -2;

	FILE *fp;
	int returnValue;
	long fileOffset;
	int __errno;

	fileOffset = offsetPage * sizePage;
	__errno = fopen_s(&fp, SWAPING_NAME, "r+");
	if (__errno == 2)
	{
		__errno = fopen_s(&fp, SWAPING_NAME, "w");
	}
	if (!__errno)
	{
		__errno = fseek(fp, fileOffset, SEEK_SET);
		if (!__errno)
		{
			int i;
			for (i = 0; i < sizePage; i++)
				fputc(writtenPage[i], fp);
			returnValue = SUCCESSFUL_CODE;
		}
		else
		{
			returnValue = UNEXPECTED_EOF;
		}
		fclose(fp);
	}
	else
	{
		returnValue = FILE_NOT_FOUND;
	}
	return returnValue;
}

void _printBuffer(VA ptr, size_t szBuffer)
{
	VA raddr, vaBuffer;
	int indexPage, offsetBlock;
	unsigned int j;
	struct block* readBlock;
	raddr = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);

	if (!raddr)
	{
		int __errno = swap(getLeastActivePageInMemory(), &pageTable[indexPage]);
		raddr = convertVirtualtoPhysicalAddress(ptr, &indexPage, &offsetBlock);
	}
	{
		for (j = 0; j < szBuffer; j++)
			printf("%c", raddr[j]);
	}
}