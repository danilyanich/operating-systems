#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
	m_init(2, 300);

	int error_code;
	struct block* chunk_1 = m_malloc(13, &error_code);
	struct block* chunk_2 = m_malloc(7, &error_code);
	struct block* chunk_3 = m_malloc(2, &error_code);

  system("pause");
}
