#pragma once
#ifndef MMEMORY_H
#define MMEMORY_H

#include <stdbool.h>
#include <stddef.h>

#define RC_SUCCESS 0
#define RC_ERR_U 1
#define RC_ERR_INPUT -1
#define RC_ERR_SF -2

#define _init(x, y) s_init(x, y)
#define _malloc(x, y) s_malloc(x, y)

typedef char* VA;	// ��� ����������� ����� ����� 

// ST is a queue that represents segments table.
typedef struct _segment_table
{
	VA va;                      // Segment VA.
	size_t l;                   // Segment length
	struct _segment_table* n;   // Physical address of next segment's VA.
} ST;

// MEMORY represents virtual address space.
typedef struct
{
	size_t pa;		// Physical address of first block.
	size_t sz;	    // Number of blocks in memory.
	ST* fs;         // Points to the first segment in the table.
} MEMORY;

/**
	@func	_malloc
	@brief	�������� ���� ������ ������������� �������

	@param	[out] ptr	    ������ �����
	@param	[in]  szBlock	������ �����

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	�������� ������
	@retval	1	����������� ������
 **/
int _malloc(VA* ptr, size_t szBlock);

/**
	@func	_free
	@brief	�������� ����� ������

	@param	[in] ptr	������ �����

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
int _free(VA ptr);

/**
	@func	_read
	@brief	������ ���������� �� ����� ������

	@param	[in] ptr	    ������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
int _read(VA ptr, char* pBuffer, size_t szBuffer);

/**
	@func	_write
	@brief	������ ���������� � ���� ������

	@param	[in] ptr	    ������ �����
	@param	[in] pBuffer	������ ������ ���� ���������� ���������
	@param	[in] szBuffer	������ ������

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	-2	������ �� ������� �����
	@retval	1	����������� ������
 **/
int _write(VA ptr, char* pBuffer, size_t szBuffer);

/**
	@func	_init
	@brief	������������� ������ ��������� ������

	@param	[in] n		���������� �������
	@param	[in] szPage	������ ��������

	� �������� 1 � 2 ����� ����� ������ ������������� ��� n*szPage

	@return	��� ������
	@retval	0	�������� ����������
	@retval	-1	�������� ���������
	@retval	1	����������� ������
 **/
int _init(int n, int szPage);

#endif  