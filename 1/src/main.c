#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory/memory.h"

void _read(struct block* chunk) {
				char buffer[100];
				int error_code;
				m_read(*chunk, buffer, chunk->size, &error_code);
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

void _write(struct block* chunk, char* data) {
				int error_code;
				m_write(*chunk,data, chunk->size, &error_code);
				if (error_code != M_ERR_OK) abort();
}

int main(int argc, char** argv) {
	m_init(1, 500);

	int error_code;

	struct block* chunk_1 = _malloc(23);
	struct block* chunk_2 = _malloc(33);
	struct block* chunk_3 = _malloc(23);
	struct block* chunk_4 = _malloc(43);
	struct block* chunk_5 = _malloc(23);

	_write(chunk_1, "Hello World!");
	_write(chunk_2, "1234567890"); 
	_write(chunk_3, "LAB COATS"); 
	_write(chunk_4, "EL PSY CONGROO");
	_write(chunk_5, "Jormungand"); 

	printf("\n    BEFORE:    \n");
	printf("----------FIRST CHUNK----------- \n");
	_read(chunk_1);

	printf("----------SECOND CHUNK--------- \n");
	_read(chunk_2);

	printf("---------THIRD CHUNK------- \n");
_read(chunk_3);

	printf("---------FOURTH CHUNK------- \n");
_read(chunk_4);

	printf("---------FIFTH CHUNK------- \n");
_read(chunk_5);

	//-----------------------------------------------

	m_free(chunk_4, &error_code, chunk_4->size);

	//-----------------------------------------------

	printf("\n    AFTER:    \n");

	printf("--------FIRST CHUNK--------- \n");
	_read(chunk_1);
 
	printf("----------SECOND CHUNK------\n");

_read(chunk_2);

	printf("---------THIRD CHUNK------- \n");

_read(chunk_3);

	printf("---------FOURTH CHUNK------- \n");

_read(chunk_4);

	printf("---------FIFTH CHUNK------- \n");
	_read(chunk_5);
}
