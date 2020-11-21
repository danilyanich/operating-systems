#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <corecrt_malloc.h>

#include "memory.h"


using namespace std;

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;
char buffer[100];
int buff_tab[100];




m_id m_malloc(int size_of_chunk, m_err_code* error) {
    if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    _g_bytes_allocated += size_of_chunk;

    *error = M_ERR_OK;

    return _g_allocator_memory + _g_bytes_allocated;
}


void m_free() {
    free(_g_allocator_memory);
    
}


void m_read(m_id read_from_id, char read_to_buffer[], int size_to_read, m_err_code* error, int chunk_number) 
{
    //есть ли чанк
    int chunk_lenth=0;
    int chunk_begin = 0;
    for (int i = 0; i < 100; i++) {
        if (buff_tab[i] == chunk_number) {
            chunk_lenth++;
        }
    }
    //cout << chunk_lenth;


    //если его нет то
    if (chunk_lenth != size_to_read) {
        //берем из памяти
        memcpy(read_to_buffer, read_from_id, size_to_read);
        //должны записать в буфер
        int empty_buf_len = 0;
        //находим пустое место
        for (int i = 0; i < 100; i++) {
            if (buff_tab[i] == 0) {
                empty_buf_len++;
                if (buff_tab[i + 1] != 0) {
                    break;
                }
            }
        }
        //cout << empty_buf_len << endl;
        //если первое пустое место подходит
        if (empty_buf_len >= size_to_read) {
            //cout << endl << "ok" << endl;
            int begin_empty = 0;
            //находим начало пустого места
            for (int i = 0; i < 100; i++) {
                if (buff_tab[i] == 0) {
                    begin_empty = i;
                    break;
                }
            }
            //cout << "begin empty" << begin_empty;
            //записываем в буфер в пустое место
            int len=0;
            for (int i = begin_empty; i < size_to_read+begin_empty; i++) 
            {
                buff_tab[i] = chunk_number;
                buffer[i] = read_to_buffer[len];
                len++;
                //printf("%lc", buffer[i]);
            }
            //printf("\n");
        }
        else {//если нет пустого места
            cout << endl << "not ok" << endl;
        }
        
    }
    else {
        //если уже есть чанк в буфере
        //cout << "exist"<<endl;
        //находим начало чанка
        for (int i = 0; i < 100; i++) {
            if (buff_tab[i] == chunk_number) {
                chunk_begin = i;
                break;
            }
            //cout << 1;
        }
        //cout << chunk_begin;
        char buff[100];
        int lenth=0;
        //записываем в темп буфер часть из буфера которая нам нужна
        for (int i = chunk_begin; i < chunk_begin + size_to_read; i++) {
            buff[lenth] = buffer[i];
            lenth++;
        }
        //отдаем обратно эту часть из буфера
        memcpy(read_to_buffer, buff, size_to_read);

        for (int i = 0; i < 100; i++) {
            //cout << buff_tab[i] << " ";
        }
        //cout << endl;

    }
    //*error = M_ERR_OK;
}

/*void m_read_buf(char buffer[],int size) {
    for (int i = 0; i < size; i++) {
        printf("%lc",buffer[i]);
    }
    printf("\n");
    
}*/

void m_write(m_id write_to_id, const void* write_from_buffer, int size_to_write, m_err_code* error) {

    memcpy(write_to_id, write_from_buffer, size_to_write);
    *error = M_ERR_OK;
   
}


void m_init(int number_of_pages, int size_of_page) {
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);

    _g_allocator_memory_size = number_of_pages * size_of_page;
    _g_allocator_memory =(char*) malloc(_g_allocator_memory_size);
    _g_bytes_allocated = 0;

    memset(buffer, 0, 100);
}