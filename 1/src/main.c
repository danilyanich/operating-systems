#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
	m_init(1, 500);

	int error_code;
	struct block* chunk_1 = m_malloc(13, &error_code);
	struct block* chunk_2 = m_malloc(20, &error_code);
	struct block* chunk_3 = m_malloc(50, &error_code);

	m_free(chunk_1, &error_code);
	m_free(chunk_2, &error_code);
	m_free(chunk_3, &error_code);
	

  system("pause");
}
