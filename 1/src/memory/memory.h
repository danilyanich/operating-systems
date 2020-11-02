#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

typedef char* m_id;							        // Тип описывающий адрес блока

#define M_ERR_OK  0									// Успешное выполнение
#define M_ERR_BAD_PARAMS -1							// Неверные параметры
#define M_ERR_ALLOCATION_OUT_OF_MEMORY -2			// Недостаточно памяти
#define M_ERR_UNKNOWN  1							// Неизвестная ошибка

/**
	@func	m_malloc
	@brief	Выделяет блок памяти определенного размера
	@param	[out] ptr				адресс блока
	@param	[in]  size_of_block		размер блока
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	нехватка памяти
	@retval	1	неизвестная ошибка
 **/
int m_malloc(m_id* ptr, int size_of_block);


/**
	@func	m_free
	@brief	Удаление блока памяти
	@param	[in] chunk_id	адресс блока
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int m_free(m_id chunk_id);


/**
	@func	m_read
	@brief	Чтение информации из блока памяти
	@param	[in] read_from_id		адресс блока
	@param	[in] read_to_buffer		адресс буфера куда копируется инфомация
	@param	[in] size_to_read		размер буфера
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int m_read(m_id read_from_id, void* read_to_buffer, int size_to_read);


/**
	@func	m_write
	@brief	Запись информации в блок памяти
	@param	[in] write_to_id		адресс блока
	@param	[in] write_from_buffer	адресс буфера куда копируется инфомация
	@param	[in] size_of_buffer		размер буфера
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	-2	доступ за пределы блока
	@retval	1	неизвестная ошибка
 **/
int m_write(m_id write_to_id, void* write_from_buffer, int size_to_write);


/**
	@func	m_init
	@brief	Инициализация модели менеджера памяти
	@param	[in] number_of_pages	количество страниц
	@param	[in] size_of_page		размер страницы
	В варианте 1 и 2 общий объем памяти расчитывается как number_of_pages * size_of_page
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int m_init(int number_of_pages, int size_of_page);

/**
	@func	m_clean
	@brief	Очищение памяти
	@return	код ошибки
	@retval	0	успешное выполнение
	@retval	-1	неверные параметры
	@retval	1	неизвестная ошибка
 **/
int m_clean();

#endif