#ifndef MMEMORY_H
#define MMEMORY_H

#include <stdbool.h>
#include <stddef.h>

#define RC_SUCCESS 0
#define RC_ERR_U 1
#define RC_ERR_INPUT -1
#define RC_ERR_SF -2

typedef char* VA;	// Тип описывающий адрес блока

/**
 	@func	_malloc	
 	@brief	Выделяет блок памяти определенного размера
	
	@param	[out] ptr	    адресс блока
	@param	[in]  szBlock	размер блока
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	нехватка памяти
	@retval	1	неизвестная ошибка
 **/
int _malloc (VA* ptr, size_t szBlock);

/**
 	@func	_free
 	@brief	Удаление блока памяти
	
	@param	[in] ptr	адресс блока
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int _free (VA ptr);

/**
 	@func	_read
 	@brief	Чтение информации из блока памяти
	
	@param	[in] ptr	    адресс блока
	@param	[in] pBuffer	адресс буфера куда копируется инфомация
	@param	[in] szBuffer	размер буфера
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int _read (VA ptr, void* pBuffer, size_t szBuffer);

/**
 	@func	_write
 	@brief	Запись информации в блок памяти
	
	@param	[in] ptr	    адресс блока
	@param	[in] pBuffer	адресс буфера куда копируется инфомация
	@param	[in] szBuffer	размер буфера
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int _write (VA ptr, void* pBuffer, size_t szBuffer);

/**
 	@func	_init
 	@brief	Инициализация модели менеджера памяти
	
	@param	[in] n		количество страниц
	@param	[in] szPage	размер страницы

	В варианте 1 и 2 общий объем памяти расчитывается как next*szPage
	
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int _init (int n, int szPage);

#endif

