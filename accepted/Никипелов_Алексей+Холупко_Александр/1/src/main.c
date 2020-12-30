#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

void panic(int error_code) {
  switch(error_code) {
    case M_ERR_ALLOCATION_OUT_OF_MEMORY: printf("Error: M_ERR_ALLOCATION_OUT_OF_MEMORY\n"); abort(); break;
    case M_ERR_ALREADY_DEALLOCATED: printf("Error: M_ERR_ALREADY_DEALLOCATED\n"); abort(); break;
    case M_ERR_INVALID_CHUNK: printf("Error: M_ERR_INVALID_CHUNK\n"); abort(); break;
    case M_ERR_OUT_OF_BOUNDS: printf("Error: M_ERR_OUT_OF_BOUNDS\n"); abort(); break;
    default: printf("Unknown error\n"); abort(); break;
  }
}

int main(int argc, char **argv) {
  m_init(1, 500);
  int error_code;

  char message_1[] = "Hello from Independent Republic of Ireland";
  int message_1_len = sizeof(message_1) / sizeof(char);
  char message_2[] = "Ira is the most powerful army in the world";
  int message_2_len = sizeof(message_2) / sizeof(char);
  char message_3[] = "GB sucks";
  int message_3_len = sizeof(message_3) / sizeof(char);

  m_id chunk_1 = m_malloc(message_1_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  m_id chunk_2 = m_malloc(message_2_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  m_id chunk_3 = m_malloc(message_3_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  m_write(chunk_1, message_1, message_1_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  m_write(chunk_2, message_2, message_2_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  m_write(chunk_3, message_3, message_3_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  char buffer[50];

  m_read(chunk_1, buffer, message_1_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  printf("%s\n", buffer);

  m_read(chunk_2, buffer, message_2_len, &error_code);

  if (error_code != M_ERR_OK) panic(error_code);
  printf("%s\n", buffer);

  m_read(chunk_3, buffer, message_3_len, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  printf("%s\n", buffer);

  m_free(chunk_1, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  m_free(chunk_2, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);

  m_free(chunk_3, &error_code);
  if (error_code != M_ERR_OK) panic(error_code);
}
