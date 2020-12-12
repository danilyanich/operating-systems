#include <stdlib.h>
#include <assert.h>
#include "memory.h"
#include "memory_model.h"

#define get_segment_end_address(s) (s->start + s->segment_length) // адрес конца сегмента
static memory_model *memoryModel;
static segment_queue *cached_record;

void check_cache(segment_queue **s)
{
    if (cached_record == *s)
    {
        *s = cached_record;
    }
}
void free_all()
{
    if (memoryModel == NULL)
    {
        return;
    }

    segment_queue *cur = memoryModel->first_segment;
    segment_queue *prev;
    while (cur->next != NULL)
    {
        prev = cur;
        cur = cur->next;

        free(prev);
        prev = NULL;
    }

    if (cur == memoryModel->first_segment)
    {
        memoryModel->first_segment = NULL;
    }

    free(cur);

    free(memoryModel);
    memoryModel = NULL;
    cached_record = NULL;
}

segment_queue *find_space(size_t s_len)
{
    assert(memoryModel->first_segment != NULL);
    // Проверка на то, что память инициализирована

    if (memoryModel->first_segment->next == NULL)
    {
        return memoryModel->first_segment;
        // Если есть только первый сегмент
    }

    segment_queue *s = memoryModel->first_segment;

    while (s->next != NULL)
    {
        if (s->next->start - get_segment_end_address(s) >= s_len)
        {
            return s;
            // Поиск подходящего по размеру сегмента
        }

        s = s->next;
    }

    if (memoryModel->memory_size - (size_t)s->start >= s_len)
    {
        return s;
        // Проверка на то что сегмент не выходит за пределы памяти
    }

    return NULL;
}

int _malloc(MemAddr *addr, size_t blockSize)
{
    if ((blockSize < 1) || (blockSize > memoryModel->memory_size))
    {
        return ERR_INPUT;
    } // Проверка на допутимые параметры

    segment_queue *s = find_space(blockSize);
    if (s == NULL)
    {
        return ERR_UNKNOWN;
    }

    if ((s == memoryModel->first_segment) && (s->segment_length == 0))
    {
        s->segment_length = blockSize;

        *addr = s->start;

        // Если память пуста, то инициализируем первый сегмент
        return SUCCESS;
    }

    segment_queue *next = s->next;
    s->next = calloc(1, sizeof(segment_queue));
    s->next->start = get_segment_end_address(s) + 1;
    s->next->segment_length = blockSize;
    s->next->next = next;
    // Если не пуста, то добавляем сегмент в конец

    *addr = s->start;

    return SUCCESS;
}

int virtual_to_segment(const MemAddr ptr, segment_queue **segment)
{
    if ((ptr < 0) || (ptr > (MemAddr)memoryModel->memory_size))
    {
        // Проверка на корректность параметров
        return ERR_SEG_FAULT;
    }

    segment_queue *s = memoryModel->first_segment;
    while (s->next != NULL)
    {
        if (ptr <= get_segment_end_address(s))
        {
            *segment = s;
            // Как только найдем конец сегмента, сдвигаем его
            return SUCCESS;
        }

        s = s->next;
    }

    return ERR_UNKNOWN;
}

size_t get_segment_physical_address(segment_queue *s)
// Получение реального (физического) адреса сегмента
{
    assert(s != NULL);
    return memoryModel->physical_address + (size_t)s->start;
}

int _free(MemAddr addr)
{
    segment_queue *segment;
    int code = virtual_to_segment(addr, &segment);
    // Помещаем содержимое виртуальной памяти в реальный (физический) сегмент
    if (code != SUCCESS)
    {
        return code;
    }

    if (segment == memoryModel->first_segment)
    {
        memoryModel->first_segment = segment->next;
        free(segment);
        return SUCCESS;
        // Если сегмент был первым, очащаем его и сдвигаем указатель
    }

    segment_queue *prev = memoryModel->first_segment;
    while (prev->next != segment)
    {
        prev = prev->next;
    }
    prev->next = segment->next;
    free(segment);
    return SUCCESS;
    // Если сегмент в любом другом месте, ищем его, очищаем, и смещаем указатель пред. сегмента
}

void get_cache_for(segment_queue *segment)
// Кэширование следующего сегмента за сегментом-параметре
{
    if (segment != NULL && segment->next != NULL)
    {
        cached_record = segment->next;
    }
}

int _read(MemAddr addr, void *bufferStart, size_t bufferSize)
{
    if ((bufferStart == NULL) || (bufferSize < 1))
    {
        // Проверка на корректность параметров
        return ERR_INPUT;
    }
    segment_queue *segment;
    int code = virtual_to_segment(addr, &segment);
    if (code != SUCCESS)
    {
        return code;
    }
    if (bufferSize < segment->segment_length)
    {
        // Если размер буфера меньше длины сегмента
        return ERR_SEG_FAULT;
    }
    MemAddr bufEl;
    MemAddr sEl;
    check_cache(&segment);
    MemAddr pa = (MemAddr)get_segment_physical_address(segment);
    for (int i = 0; i < bufferSize; i++)
    {
        bufEl = ((MemAddr)bufferStart + i);
        sEl = (pa + i);
        assert(sEl != NULL);
        assert(bufEl != NULL);
        *bufEl = *sEl;
    } // Побайтовое чтение
    get_cache_for(segment);
    return SUCCESS;
}

int _write(MemAddr addr, void *bufferStart, size_t bufferSize)
{
    if ((bufferStart == NULL) || (bufferSize < 1))
    {
        return ERR_INPUT;
    }
    segment_queue *segment;
    int code = virtual_to_segment(addr, &segment);
    if (code != SUCCESS)
    {
        return code;
    }

    if (bufferSize > segment->segment_length)
    {
        return ERR_SEG_FAULT;
    }
    MemAddr bufEl;
    MemAddr sEl;
    check_cache(&segment);
    MemAddr pa = (MemAddr)get_segment_physical_address(segment);
    for (int i = 0; i < bufferSize; i++)
    {
        bufEl = ((MemAddr)bufferStart + i);
        sEl = (pa + i);
        assert(sEl != NULL);
        assert(bufEl != NULL);
        *sEl = *bufEl;
    } // Побайтовая запись

    return SUCCESS;
}

int _init(int amountOfPages, int pageSize)
{
    if ((amountOfPages < 1) || (pageSize < 1))
    {
        // Проверка корректности параметров
        return ERR_INPUT;
    }

    // Инициализация memoryModel и первого сегмента
    free_all();
    memoryModel = calloc(1, sizeof(memory_model));
    memoryModel->memory_size = amountOfPages * pageSize;
    memoryModel->physical_address = (size_t)calloc(memoryModel->memory_size, 1);
    memoryModel->first_segment = calloc(1, sizeof(segment_queue));

    return SUCCESS;
}