#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert.h"
#include "memory/memory.h"

void _read(struct block* block) {
				char buffer[100];
				int error_code;
				m_read(block, buffer, block->size, &error_code);
				if (error_code != M_ERR_OK) abort();
				printf("%s\n", buffer);
				memset(buffer, 0, sizeof(buffer));
}

struct block* _malloc(int size) {
				int error_code;
				struct block* newBlock = m_malloc(size, &error_code);
				if (error_code != M_ERR_OK) abort();
				return newBlock;
}

void _write(struct block* block, char* data) {
				int error_code;
				m_write(block,data, block->size, &error_code);
				if (error_code != M_ERR_OK) abort();
}


int main(int argc, char** argv) {

	m_init(1, 500);

	int error_code;
	 struct block* block1 = _malloc(23);
	struct block* block2 = _malloc(33);
	struct block* block3 = _malloc(23);
	struct block* block4 = _malloc(33);
	struct block* block5 = _malloc(43);
	_write(block1, "Dasha");
	_write(block2, "Vadzim"); 
	_write(block3, "OS 1 lab"); 
	_write(block4, "success");
	_write(block5, "please accept our lab"); 

	printf("----------FIRST----------- \n");
	_read(block1);

	printf("----------SECOND--------- \n");
	_read(block2);

	printf("---------THIRD------------ \n");
_read(block3);

	printf("---------FOURTH------------ \n");
_read(block4);

	printf("---------FIFTH------------ \n");
_read(block5);
m_free(block4, &error_code, block4->size);
printf("-----------FOURTH-------------\n");
_read(block4);
return 0;
}
