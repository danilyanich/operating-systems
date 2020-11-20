#ifndef MEMORY_H
#define MEMORY_H

#define M_ERR_OK 0 // Everything is ok - все ок
#define M_ERR_ALLOCATION_OUT_OF_MEMORY 1 // Not enough memory for allocation - недостаточно памяти
#define M_ERR_ALREADY_DEALLOCATED 2 // The chunk was already deallocated - уже освобождено
#define M_ERR_INVALID_CHUNK 3 // The chunk is invalid, the operation did not succeed - фрагмент недействителен, операция не успешна
#define M_ERR_OUT_OF_BOUNDS 4 // The read/write operation out of bounds - выход за пределы


typedef void* m_id; // Identifier of sandbox memory chunk
typedef int m_err_code; // Error code of sandbox memory


//http://vdenis.ru/all/pishem-prostoy-menedzher-pamyati/
//https://embedded2day.com/2017/05/13/memory_manager/
//http://www.realcoding.net/article/view/2747


struct box {			//используем struct так как  нельзя освобождать случайные участки памяти!!!
	m_id memory;
	int size;
	struct box* prev;
	struct box* next;
	int end_or_not;		

};

int put_box(struct box*, struct box*, int);

// Allocates a chunk in sandbox memory - ВЫДЕЛЕНИЕ
// @param size_of_chunk Desired size in bytes to be allocated in sandbox memory - ЖЕЛАЕМЫЙ РАЗМЕР В БАЙТАХ
// @param error_code [out] M_ERR_OK, M_ERR_ALLOCATION_OUT_OF_MEMORY - ????
// @return An identifier for newly allocated chunk - АЙДИ ВЫДЕЛЕННОГО БЛОКА
m_id m_malloc(int size_of_chunk, m_err_code* error_code);


// Deallocates a chunk in sandbox memory - ОСВОБОЖДЕНИЕ
// @param chunk_id Chunk identifier - АЙДИ 
// @param error_code [out] M_ERR_OK, M_ERR_ALREADY_DEALLOCATED, M_ERR_INVALID_CHUNK - ОШИБКИ
void m_free(m_id chunk_id, m_err_code* error_code);


// Reads from chunk to a buffer - ИЗ ВЫДЕЛЕННОГО В БУФФЕР
// @param read_from_id The chunk to read from - АЙДИ БЛОКА, ОТКУДА БРАТЬ
// @param read_to_buffer [out] The buffer to store data into - ДЛЯ ХРАНЕНИЯ 
// @param size_to_read Size of data in bytes to read from chunk - РАЗМЕР БЛОКА В БАЙТАХ ДЛЯ ЧТЕНИЯ
// @param error_code [out] M_ERR_OK, M_ERR_INVALID_CHUNK, M_ERR_OUT_OF_BOUNDS - ОШИБКИ
void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error_code);


// Writes from buffer to a chunk - ВЫВОД НА ПЕЧАТЬ
// @param write_to_id The chunk to store data into - АЙДИ БЛОКА ДЛЯ ХРАНЕНИЯ ВЫВОДИМОГО
// @param write_from_buffer The buffer to read from - ДЛЯ ХРАНЕНИЯ
// @param size_to_write Size of data in bytes to be stored into chunk - РАЗМЕР В БАЙТАХ ДЛЯ ВЫВОДА
// @param error_code [out] M_ERR_OK, M_ERR_INVALID_CHUNK, M_ERR_OUT_OF_BOUNDS - ОШИБКИ
void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error_code);


// Initializes sandbox memory allocator. Usually it is number_of_pages*size_of_page.
// @param number_of_pages Number of pages to allocate
// @param size_of_page Size of the page
void m_init(int number_of_pages, int size_of_page);


#endif /* MEMORY_H */