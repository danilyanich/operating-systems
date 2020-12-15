#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#include "memory/memory.h"

int main(int argc, char **argv) {
  m_init(20, 20);

  int error_code;

  m_id chunk_1 = m_malloc(13, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id chunk_2 = m_malloc(20, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id chunk_3 = m_malloc(50, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_id chunk_4 = m_malloc(150, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_write(chunk_1, "Salam Aleikum", 13, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_write(chunk_2, "Dirizabl aga chita", 18, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_write(chunk_3, "Kono Giorno Giovanna yume ga aru", 32, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_write(chunk_4, "Welcome to Camp Navarro. So, you're the new replacement... You are out of uniform, soldier! Where is your power armor?", 118, &error_code);
  if (error_code != M_ERR_OK) abort();

  char buffer[150];

  printf("\nReading...\n\n");

  m_read(chunk_1, buffer, 13, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(chunk_2, buffer, 18, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(chunk_3, buffer, 32, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  m_read(chunk_4, buffer, 118, &error_code);
  if (error_code != M_ERR_OK) abort();
  printf("%s\n", buffer);

  printf("\nReading complite\n\n");

  m_free(chunk_1, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(chunk_2, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(chunk_3, &error_code);
  if (error_code != M_ERR_OK) abort();

  m_free(chunk_4, &error_code);
  if (error_code != M_ERR_OK) abort();

  _getch();
}
