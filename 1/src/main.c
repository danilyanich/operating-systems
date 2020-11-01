#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
    m_init(1, 500);

    int error_code;
/*    m_id a = m_malloc(2, &error_code);
    m_id b = m_malloc(7, &error_code);
    m_id c = m_malloc(6, &error_code);
    m_free(a, &error_code);
    m_id d = m_malloc(5, &error_code);
    m_free(b, &error_code);
    m_malloc(10, &error_code);

    unsigned char buffer[] = {0b10001111, 0b10101010, 0b11111111};

    m_write(d + 1, buffer, 3, &error_code);

    unsigned char buff2[3];

    m_read(c, buff2, 3, &error_code);

    dump();

    print_binary(buff2[0]);
    printf("\n");
    print_binary(buff2[1]);
    printf("\n");
    print_binary(buff2[2]);
    fflush(stdout);
  */

  m_id chunk_1 = m_malloc(13, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id chunk_2 = m_malloc(20, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id chunk_3 = m_malloc(50, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_write(chunk_1, "Hello World!", 13, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_write(chunk_2, "Operating Systems", 18, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_write(chunk_3, "Super dumb memory allocator", 28, &error_code);
  if (error_code != M_ERR_OK) abort();

  char buffer[50];

  m_read(chunk_1, buffer, 13, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(chunk_2, buffer, 18, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(chunk_3, buffer, 28, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_free(chunk_1, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(chunk_2, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(chunk_3, &error_code);
  if (error_code != M_ERR_OK) abort();
}
