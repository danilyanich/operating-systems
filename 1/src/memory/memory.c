#include <stdlib.h>
#include <string.h>

#include "memory.h"

memory* _g_allocator_memory;

int _g_bytes_allocated = 0;

m_id m_malloc(int size_of_page, m_err_code* error) {
    // если уже не лезет в память, то пишем ошибку и выходим
    if (_g_bytes_allocated + size_of_page > _g_allocator_memory->size) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    // выделяем реальную память под саму структуру страницы и обнуляем у нее все поля
    page* new_page = malloc(sizeof(page));
    new_page->size = size_of_page;
    new_page->dataSize = 0;
    new_page->next = NULL;
    new_page->prev = NULL;
    new_page->isFilled = false;

    if (_g_allocator_memory->next == NULL) {                       // добавляем страницу в начало, если она первая
        _g_allocator_memory->next = new_page;
        new_page->data = _g_allocator_memory->memory;
    }
    else {
        page* last_page = _g_allocator_memory->next;                // иначе ищем последнюю страницу и добавляем после нее новую (как next)
        while (true) {
            if (last_page->next == NULL) {
                break;  // нашли последнюю
            }
            last_page = last_page->next;
        }
      
        new_page->prev = last_page; // новой странице как предыдущую пишет last_page
        new_page->data = last_page->data + last_page->size; // смещаем указатель на данные на размер страницы, т.е. если страница 1 начинала свои данные с 0 байт, то страница 2 хранит свои данные с 50 байт

        last_page->next = new_page; // last-page как следующую страницу пишем новую
    }

    *error = M_ERR_OK;
    _g_bytes_allocated += size_of_page;
    return new_page;
}


void m_free(m_id ptr, m_err_code* error) {
    ptr->isFilled = false;  // ставим странице флаг, что она пустая

    _g_bytes_allocated -= ptr->size;
    
    *error = M_ERR_OK;
}


void m_read(m_id read_from_id, void* read_to_buffer, m_err_code* error) {
    memcpy(read_to_buffer, read_from_id->data, read_from_id->dataSize); // считываем с переданной страницы столько данных, сколько в ней хранится (dataSize)
    *error = M_ERR_OK;
}


m_id m_write(memory* memory_to_write, void* write_from_buffer, int size_to_write, m_err_code* error) {
    page* last_page = memory_to_write->next;    // собсно опять ищем последнюю незаписанную страницу (isFilled == false)
    while (true) {
        if (last_page->isFilled == false && last_page->size >= size_to_write) {
            break;  // нашли
        }
        last_page = last_page->next;
    }

    if (last_page != NULL) {    // если нашли пустую страницу, то пишем в нее
        memcpy(last_page->data, write_from_buffer, size_to_write);
        last_page->dataSize = size_to_write;
        last_page->isFilled = true;
        *error = M_ERR_OK;
        return last_page;
    } else {
        *error = M_ERR_OUT_OF_BOUNDS;
        return NULL;
    }
}


memory* m_init(int size_of_memory, int size_of_page, m_err_code* error) {
    _g_allocator_memory = calloc(1, sizeof(memory));  // выделяем память под саму структуру памяти
    _g_allocator_memory->size = size_of_memory; // записываем общий размер памяти
    _g_allocator_memory->memory = calloc(1, size_of_memory);  // выделяем память под сами данные

    // делим всю память на страницы заданного размера и выделяем под каждую страницу виртуальную память
    m_id next_page = _g_allocator_memory;   
    int number_of_pages = size_of_memory / size_of_page;

    for (int i = 0; i < number_of_pages; i++) {
        next_page->next = m_malloc(size_of_page, error);  // собсно выделяем вирт память
        next_page = next_page->next;
    }

    if (_g_allocator_memory == NULL) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    _g_bytes_allocated = 0;
    *error = M_ERR_OK;
    return _g_allocator_memory;
}
