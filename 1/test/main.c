#include <stdlib.h>
#include <stdio.h>

#include "memory.h"

int main(int argc, char** argv) {
	m_init(1, 500);

	int error_code;


	struct box* chunk_1 = m_malloc(11, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct box* chunk_2 = m_malloc(20, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct box* chunk_3 = m_malloc(50, &error_code);
	if (error_code != M_ERR_OK) abort();


//задания для вписывания
//тестовые из примера

	m_write(chunk_1, "Zabastovka", 11, &error_code);
	if (error_code != M_ERR_OK) abort();

	m_write(chunk_2, "Operating Systems", 18, &error_code);
	if (error_code != M_ERR_OK) abort();

	m_write(chunk_3, "Super dumb memory allocator", 28, &error_code);
	if (error_code != M_ERR_OK) abort();

	char buffer[50];


	m_read(chunk_1, buffer, 11, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);

	m_read(chunk_2, buffer, 18, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);

	m_read(chunk_3, buffer, 28, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);


//выделение и проверка корректного удаления 

	m_free(chunk_1, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct box* chunk_4_del = m_malloc(5, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct box* chunk_5 = m_malloc(9, &error_code);
	if (error_code != M_ERR_OK) abort();

	m_free(chunk_4_del, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct box* chunk_6 = m_malloc(4, &error_code);
	if (error_code != M_ERR_OK) abort();

}