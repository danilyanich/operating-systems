#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
	m_init(2, 300);

	int error_code;
	void* buff[30];

	struct block* chunk_1 = m_malloc(25, &error_code);
	struct block* chunk_2 = m_malloc(30, &error_code);
	struct block* chunk_3 = m_malloc(50, &error_code);

	m_write(chunk_1, "Hey from Alesya and Rita", 25, &error_code);
	m_write(chunk_2, "Hello Daniil Yanovich", 22, &error_code);
	m_write(chunk_3, "Have a nice day", 16, &error_code);

	m_read(chunk_1, buff, 25, &error_code);
	printf("%s\n", buff);
	
	m_read(chunk_2, buff, 22, &error_code);
	printf("%s\n", buff);
	
	m_read(chunk_3, buff, 16, &error_code);
	printf("%s\n", buff);
	
	m_free(chunk_1, &error_code);
	m_free(chunk_2, &error_code);
	m_free(chunk_3, &error_code);

	system("pause");
}
