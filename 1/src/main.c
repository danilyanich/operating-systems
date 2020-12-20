#include <stdlib.h>
#include <stdio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
  int error_code;

  // allocating memory
  m_init(500, &error_code);
  if (error_code != M_ERR_OK) abort();


  // allocating segments
  m_id segment_1 = m_malloc(13, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id segment_2 = m_malloc(20, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id segment_3 = m_malloc(50, &error_code);
  if (error_code != M_ERR_OK) abort();


  // writing to segments
  m_write(segment_1, "OS Ventskute", 13, &error_code);
  if (error_code != M_ERR_OK) abort();
					  
  m_write(segment_2, "821703 4-iy variant", 20, &error_code);
  if (error_code != M_ERR_OK) abort();
					  
  m_write(segment_3, "Segmented memory allocator!", 28, &error_code);
  if (error_code != M_ERR_OK) abort();

  // reading into buffer from segments
  char buffer[50];

  m_read(segment_1, buffer, 13, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(segment_2, buffer, 20, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(segment_3, buffer, 28, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n\n", buffer);

  
  // rewriting second segment with another

  m_free(segment_2, &error_code);
  m_id segment_33 = m_malloc(18, &error_code);
  m_write(segment_33, "vvvvvv 4 vvvvvv!", 17, &error_code);

  segment* link = segment_1;
  while (link != NULL) {
	  m_read(link, buffer, link->size, &error_code);
	  printf("'%s' %s -> ", buffer, link->isFilled ? "filled" : "empty");
	  link = link->next;
  };

  //free memory
  m_free(segment_1, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(segment_2, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(segment_3, &error_code);
  if (error_code != M_ERR_OK) abort();
}
