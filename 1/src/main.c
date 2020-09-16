#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
  m_init(1, 500);

  m_id chunk_1;
  m_malloc(&chunk_1, 13);

  m_id chunk_2;
  m_malloc(&chunk_2, 20);

  m_id chunk_3;
  m_malloc(&chunk_3, 50);

  m_write(chunk_1, "Hello World!", 13);
  m_write(chunk_2, "Operating Systems", 18);
  m_write(chunk_3, "Super dumb memory allocator", 28);

  char buffer[50];

  m_read(chunk_1, buffer, 13);
  printf("%s\n", buffer);

  m_read(chunk_2, buffer, 18);
  printf("%s\n", buffer);

  m_read(chunk_3, buffer, 28);
  printf("%s\n", buffer);
}
