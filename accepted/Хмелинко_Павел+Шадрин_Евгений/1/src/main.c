#include <stdlib.h>
#include <stdio.h>
#include "memory/memory.h"

int main(int argc, char **argv) {
   m_init(1, 500);
   char buffer[50];
   int error_code;


  struct mem_chunk* chunk_1 = m_malloc(13, &error_code);
  if (error_code != M_ERR_OK) abort();



  struct mem_chunk* chunk_2 = m_malloc(18, &error_code);
  if (error_code != M_ERR_OK) abort();

  struct mem_chunk* chunk_3 = m_malloc(28, &error_code);
  if (error_code != M_ERR_OK) abort();

  struct mem_chunk *chunk_4 = m_malloc(17, &error_code);
    if (error_code != M_ERR_OK) abort();

   struct mem_chunk *chunk_5 = m_malloc(14, &error_code);
   if (error_code != M_ERR_OK) abort();




    m_write(chunk_1, "Hello World!", 13, &error_code);
    if (error_code != M_ERR_OK) abort();

  m_write(chunk_2, "Operating Systems", 18, &error_code);
  if (error_code != M_ERR_OK) abort();


  m_write(chunk_3, "Super dumb memory allocator", 28, &error_code);
  if (error_code != M_ERR_OK) abort();

    m_write(chunk_4, "Boss of this gym", 17, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_write(chunk_5, "Boy next door", 14, &error_code);
    if (error_code != M_ERR_OK) abort();

    m_init(1, 200);



   m_read(chunk_3, buffer, 28, &error_code);
   if (error_code != M_ERR_OK) abort();
   printf("%s\n", buffer);
}
