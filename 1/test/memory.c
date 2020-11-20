#include <stdlib.h>
#include <string.h>

#include "memory.h"


char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

struct box* top = NULL;
struct box* begin = NULL;

m_id m_malloc(int size_of_chunk, m_err_code* error) {

    //не изменено - выход за пределы
    if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size)
    {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
        return NULL;
    }

    struct box* temp = malloc(sizeof(*temp));       //http://cppstudio.com/post/9088/
    struct box* this_box = begin;
    int result = 0;

    if (begin == NULL) {
        begin = temp;
        temp->memory = _g_allocator_memory + size_of_chunk;

    }


    else {
        result = put_box(temp, this_box, size_of_chunk);
    }
    if (result) {
        //printf("\n%d, %d ", size_of_chunk);
        printf("\n%d, %d ", (int)_g_allocator_memory, size_of_chunk);
        printf("\n%d ", (int)temp->memory);
        *error = M_ERR_OK;
        return (m_id)temp;
    }


    else 
    {
        temp->size = size_of_chunk;
        temp->prev = top;
        temp->next = NULL;
        temp->end_or_not = 1;
        if (temp->prev != NULL) 
        {
            temp->memory = (char*)top->memory + size_of_chunk;
            temp->prev->next = temp;
            temp->prev->end_or_not = 0;
        }
        top = temp;
        //printf("\n%d, %d ", size_of_chunk);
        printf("\n%d, %d ", (int)_g_allocator_memory, size_of_chunk);
        printf("\n%d ", (int)temp->memory);
    } 
    *error = M_ERR_OK;      //ок
    return (m_id)temp;
}

int put_box(struct box* temp, struct box* this_box, int size_of_chunk) {
    //размерность
    if ((char*)begin->memory - _g_allocator_memory == size_of_chunk) 
    {
        temp->memory = _g_allocator_memory + size_of_chunk;
        temp->size = size_of_chunk;
        temp->next = begin;
        temp->prev = NULL;
        temp->end_or_not = 0;                                           

        begin->prev = temp;
        begin = temp;
        return 1;
    }
    else if ((char*)begin->memory - _g_allocator_memory > size_of_chunk) 
    {
        temp->memory = _g_allocator_memory + size_of_chunk;
        temp->size = size_of_chunk;
        temp->next = begin;
        temp->prev = NULL;
        temp->end_or_not = 1;

        begin->prev = temp;
        begin = temp;
        return 1;
    }

    while (this_box != top) {

        if (this_box->end_or_not == 1 && this_box->next != NULL)
        {
            //следующая есть
            if ((int)this_box->next->memory - (int)this_box->memory - this_box->next->size > size_of_chunk)
            {

                temp->memory = (char*)this_box->memory + size_of_chunk;
                temp->size = size_of_chunk;
                temp->end_or_not = 1;
                temp->next = this_box->next;
                temp->prev = this_box;

                this_box->end_or_not = 0;
                this_box->next->prev = temp;
                this_box->next = temp;
                return 1;
            }
            //следующей нет
            else if ((int)this_box->next->memory - (int)this_box->memory - this_box->next->size == size_of_chunk)
            {
                temp->memory = (char*)this_box->memory + size_of_chunk;
                temp->size = size_of_chunk;
                this_box->end_or_not = 0;
                temp->end_or_not = 0;
                temp->next = this_box->next;
                temp->prev = this_box;

                this_box->next->prev = temp;
                this_box->next = temp;
                return 1;
            }
        }
        this_box = this_box->next;

    }
    return 0;
}

//https://ravesli.com/urok-193-std-unique_ptr/#toc-0
//https://ravesli.com/urok-189-umnye-ukazateli-i-semantika-peremeshheniya/
void m_free(m_id ptr, m_err_code* error) {

    struct box* ptr_new = ptr;

    //уже освободили
    if (ptr == NULL) {
        *error = M_ERR_ALREADY_DEALLOCATED;
        return;
    }

    struct box* temp;

    //все три ситуации!!!
    if (ptr_new->memory == top->memory)
    {
        temp = ptr_new;
        ptr_new->prev->next = temp->next;
        ptr_new->next->prev = temp->prev;
        ptr_new->prev->end_or_not = 1;
        free(ptr_new);      //1
    }
    else if (ptr_new->memory == begin->memory)
    {
        temp = begin;
        begin = ptr_new->next;
        free(temp);
    }
    else
    {
        temp = top;
        top = ptr_new->prev;
        ptr_new->prev->end_or_not = 1;
        free(temp);
    }
    *error = M_ERR_OK;      //ок
}

//ситуация - выход за пределы вместимости
void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {
    struct box* ptr_new = read_from_id;

    if (size_to_read > ptr_new->size) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }
    if (&read_from_id != NULL && ptr_new->memory != NULL)
    {
        memcpy(read_to_buffer, ptr_new->memory, size_to_read);
    }

    *error = M_ERR_OK;
}

//ситуация - выход за пределы вместимости
void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    struct box* ptr_new = write_to_id;

    if (size_to_write > ptr_new->size) {
        *error = M_ERR_OUT_OF_BOUNDS;
        return;
    }

    memcpy(ptr_new->memory, write_from_buffer, size_to_write);
    *error = M_ERR_OK;
}

//не трогать
void  m_init(int number_of_pages, int size_of_page) {
    if (_g_allocator_memory != NULL) free(_g_allocator_memory);

    _g_allocator_memory_size = number_of_pages * size_of_page;
    _g_allocator_memory = malloc(_g_allocator_memory_size);
    _g_bytes_allocated = 0;


}