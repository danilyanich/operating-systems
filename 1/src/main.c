#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
<<<<<<< HEAD
  int error_code = M_ERR_OK;

  // выделяем под память 500 байт и задаем страницы по 50 байт
  memory* memory = m_init(500, 50, &error_code);
  printf("Initiated paged memory of size 500 bytes with page size of 50 bytes\n");
  if (error_code != M_ERR_OK) abort();

  // writing to pages
  m_id page_1 = m_write(memory, "OS Pagination", 14, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id page_2 = m_write(memory, "821703 3-iy variant", 20, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id page_3 = m_write(memory, "Paginated memory allocator!", 28, &error_code);
  if (error_code != M_ERR_OK) abort();

  // reading into buffer from pages
  char buffer[50];

  m_read(page_1, buffer, &error_code);	// считываем в буфер данные страницы
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(page_2, buffer, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(page_3, buffer, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n\n", buffer);
  
  printf("rewriting second page with another data\n\n");	// после этого перезапишем вторую страницу другой строкой

  m_free(page_2, &error_code);	// чистим 2-ю страницу 
  m_id page_22 = m_write(memory, "rewrite 2nd page", 17, &error_code);	// пишем в нее новую строку

  m_id link = memory->next;	// пройдемся по всей памяти и выведем все страницы подряд, и сколько занято в каждой странице памяти
  while (link != NULL) {
	  buffer[0] = '\0';
	  m_read(link, buffer, &error_code);
	  printf("'%s' (filled %d/%d) -> ", buffer, link->dataSize, link->size);
	  link = link->next;
  };

  //free memory
  m_free(page_1, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(page_2, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(page_3, &error_code);
=======
  m_init(1, 500);

  int error_code;

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
>>>>>>> 396c44bd111d0e33385da953549e4e420a8c0977
  if (error_code != M_ERR_OK) abort();
}
