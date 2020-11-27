#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
  m_init(1, 500);
  int error_code;
  m_id chunk_1 = m_malloc(42, &error_code);

  if (error_code != M_ERR_OK) abort();

  m_id chunk_2 = m_malloc(23, &error_code);

  if (error_code != M_ERR_OK) abort();

  m_id chunk_3 = m_malloc(50, &error_code);

  if (error_code != M_ERR_OK) {
      printf("%d", error_code);
      abort();
  }

  m_write(chunk_1, "Hello from Independent Republic of Ireland", 42, &error_code);

  if (error_code != M_ERR_OK) {
      printf("error: %d", error_code);
      abort();
  }

  m_write(chunk_2, "Ira is the most powerful army in the world", 20, &error_code);

  if (error_code != M_ERR_OK) {
      printf("error: %d", error_code);
      abort();
  }

  m_write(chunk_3, "GB sucks", 30, &error_code);

  if (error_code != M_ERR_OK) {
      printf("error: %d", error_code);
      abort();
  }

  char buffer[50];

  m_read(chunk_1, buffer, 42, &error_code);

  if (error_code != M_ERR_OK) {
      printf("error: %d", error_code);
      abort();
  }

  printf("%s\n", buffer);

  m_read(chunk_2, buffer, 20, &error_code);

  if (error_code != M_ERR_OK) abort();

  printf("%s\n", buffer);

  m_read(chunk_3, buffer, 30, &error_code);

  if (error_code != M_ERR_OK) abort();

  printf("%s\n", buffer);

  m_free(chunk_1, &error_code);

  if (error_code != M_ERR_OK) abort();

  m_free(chunk_2, &error_code);

  if (error_code != M_ERR_OK) abort();

// LOL KEK CHEBUREK

  m_free(chunk_3, &error_code);

  if (error_code != M_ERR_OK) abort();
}
