#include <stdlib.h>
#include <string.h>

#include "memory.h"

<<<<<<< HEAD
memory* _g_allocator_memory;

int _g_bytes_allocated = 0;

m_id m_malloc(int size_of_page, m_err_code* error) {
    // ���� ��� �� ����� � ������, �� ����� ������ � �������
    if (_g_bytes_allocated + size_of_page > _g_allocator_memory->size) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    // �������� �������� ������ ��� ���� ��������� �������� � �������� � ��� ��� ����
    page* new_page = malloc(sizeof(page));
    new_page->size = size_of_page;
    new_page->dataSize = 0;
    new_page->next = NULL;
    new_page->prev = NULL;
    new_page->isFilled = false;

    if (_g_allocator_memory->next == NULL) {                       // ��������� �������� � ������, ���� ��� ������
        _g_allocator_memory->next = new_page;
        new_page->data = _g_allocator_memory->memory;
    }
    else {
        page* last_page = _g_allocator_memory->next;                // ����� ���� ��������� �������� � ��������� ����� ��� ����� (��� next)
        while (true) {
            if (last_page->next == NULL) {
                break;  // ����� ���������
            }
            last_page = last_page->next;
        }
      
        new_page->prev = last_page; // ����� �������� ��� ���������� ����� last_page
        new_page->data = last_page->data + last_page->size; // ������� ��������� �� ������ �� ������ ��������, �.�. ���� �������� 1 �������� ���� ������ � 0 ����, �� �������� 2 ������ ���� ������ � 50 ����

        last_page->next = new_page; // last-page ��� ��������� �������� ����� �����
    }

    *error = M_ERR_OK;
    _g_bytes_allocated += size_of_page;
    return new_page;
}


void m_free(m_id ptr, m_err_code* error) {
    ptr->isFilled = false;  // ������ �������� ����, ��� ��� ������

    _g_bytes_allocated -= ptr->size;
    
    *error = M_ERR_OK;
}


void m_read(m_id read_from_id, void* read_to_buffer, m_err_code* error) {
    memcpy(read_to_buffer, read_from_id->data, read_from_id->dataSize); // ��������� � ���������� �������� ������� ������, ������� � ��� �������� (dataSize)
    *error = M_ERR_OK;
}


m_id m_write(memory* memory_to_write, void* write_from_buffer, int size_to_write, m_err_code* error) {
    page* last_page = memory_to_write->next;    // ������ ����� ���� ��������� ������������ �������� (isFilled == false)
    while (true) {
        if (last_page->isFilled == false && last_page->size >= size_to_write) {
            break;  // �����
        }
        last_page = last_page->next;
    }

    if (last_page != NULL) {    // ���� ����� ������ ��������, �� ����� � ���
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
    _g_allocator_memory = calloc(1, sizeof(memory));  // �������� ������ ��� ���� ��������� ������
    _g_allocator_memory->size = size_of_memory; // ���������� ����� ������ ������
    _g_allocator_memory->memory = calloc(1, size_of_memory);  // �������� ������ ��� ���� ������

    // ����� ��� ������ �� �������� ��������� ������� � �������� ��� ������ �������� ����������� ������
    m_id next_page = _g_allocator_memory;   
    int number_of_pages = size_of_memory / size_of_page;

    for (int i = 0; i < number_of_pages; i++) {
        next_page->next = m_malloc(size_of_page, error);  // ������ �������� ���� ������
        next_page = next_page->next;
    }

    if (_g_allocator_memory == NULL) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    _g_bytes_allocated = 0;
    *error = M_ERR_OK;
    return _g_allocator_memory;
=======

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;


m_id m_malloc(int size_of_chunk, m_err_code* error) {
  if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
    *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
    return NULL;
  }

  _g_bytes_allocated += size_of_chunk;

  *error = M_ERR_OK;
  return _g_allocator_memory + _g_bytes_allocated;
}


void m_free(m_id ptr, m_err_code* error) {
  *error = M_ERR_OK;
}


void m_read(m_id read_from_id,void* read_to_buffer, int size_to_read, m_err_code* error) {
  memcpy(read_to_buffer, read_from_id, size_to_read);
  *error = M_ERR_OK;
}


void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
  memcpy(write_to_id, write_from_buffer, size_to_write);
  *error = M_ERR_OK;
}


void m_init(int number_of_pages, int size_of_page) {
  if (_g_allocator_memory != NULL) free(_g_allocator_memory);

  _g_allocator_memory_size = number_of_pages * size_of_page;
  _g_allocator_memory = malloc(_g_allocator_memory_size);
  _g_bytes_allocated = 0;
>>>>>>> 396c44bd111d0e33385da953549e4e420a8c0977
}
