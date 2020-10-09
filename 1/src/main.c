#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "memory/memory.h"


int main(int argc, char** argv) {
	m_init(1, 500);

	int error_code;

	struct block* chunk_1 = m_malloc(23, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct block* chunk_2 = m_malloc(33, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct block* chunk_3 = m_malloc(23, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct block* chunk_4 = m_malloc(43, &error_code);
	if (error_code != M_ERR_OK) abort();

	struct block* chunk_5 = m_malloc(23, &error_code);
	if (error_code != M_ERR_OK) abort();

	char buffer[100];

	m_write(*chunk_1, "Hello World!", 23, &error_code);
	if (error_code != M_ERR_OK) abort();

	m_write(*chunk_2, "1234567890", 33, &error_code);
	if (error_code != M_ERR_OK) abort();

	m_write(*chunk_3, "LAB COATS ARE AWESOME", 23, &error_code);
	if (error_code != M_ERR_OK) abort();

	m_write(*chunk_4, "EL PSY CONGROO", 43, &error_code);
	if (error_code != M_ERR_OK) abort();

	m_write(*chunk_5, "TEST", 23, &error_code);
	if (error_code != M_ERR_OK) abort();

	printf("\n    BEFORE:    \n");
	printf("----------FIRST CHUNK----------- \n");

	m_read(*chunk_1, buffer, 23, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("----------SECOND CHUNK--------- \n");

	m_read(*chunk_2, buffer, 33, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("---------THIRD CHUNK------- \n");

	m_read(*chunk_3, buffer, 23, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("---------FOURTH CHUNK------- \n");

	m_read(*chunk_4, buffer, 43, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("---------FIFTH CHUNK------- \n");

	m_read(*chunk_5, buffer, 23, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	//-----------------------------------------------

	m_free(chunk_2, &error_code, 33);

	//-----------------------------------------------

	printf("\n    AFTER:    \n");

	printf("--------FIRST CHUNK--------- \n");

	m_read(*chunk_1, buffer, 23, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("----------SECOND CHUNK------\n");

	m_read(*chunk_2, buffer, 33, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("---------THIRD CHUNK------- \n");

	m_read(*chunk_3, buffer, 23, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("---------FOURTH CHUNK------- \n");

	m_read(*chunk_4, buffer, 43, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	printf("---------FIFTH CHUNK------- \n");

	m_read(*chunk_5, buffer, 23, &error_code);
	if (error_code != M_ERR_OK) abort();
	printf("%s\n", buffer);
	memset(buffer, 0, sizeof(buffer));
}
