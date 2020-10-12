#include <stdlib.h>
#include <stdio.h>
#include "memory.h"

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

struct block* top = NULL;
struct block* begin = NULL;


struct block* m_malloc(int size_of_chunk, m_err_code* error) {
	if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
		*error = M_ERR_ALLOCATION_OUT_OF_MEMORY;
	}

	_g_bytes_allocated += size_of_chunk;

	char* ptr_new = _g_allocator_memory + _g_bytes_allocated;
	struct block* tmp = malloc(sizeof(*tmp));

	tmp->size = size_of_chunk;
	tmp->start = ptr_new;
	tmp->prev = &(*top);
	if (tmp->prev!=NULL) tmp->prev->next=tmp;
	top = tmp;
	if (begin == NULL) begin = tmp;
	*error = M_ERR_OK;
	return tmp;
}

void m_free(struct block* ptr, m_err_code* error, int size) {
	if (ptr == NULL) { *error = M_ERR_ALREADY_DEALLOCATED; }
	struct block* q = top;
	struct block* prev = NULL;
	struct block* prev2 = NULL;
	struct block* tmp;
	int move=0;
	char* tmp_addr1;
	char* tmp_addr2;
	char buffer[100];
	
	
	if (ptr->start == top->start) {
		tmp = top;
		top = ptr->prev;
		memset(tmp->start, 0, size);
		return;
	}

	if (ptr->start == begin->start) {
		tmp = begin;
		while (q != NULL) {
			if (q->start == tmp->start) {
				begin = prev;
				memcpy(q->start, prev->start, _g_bytes_allocated - ((int)q->start - (int)_g_allocator_memory));
				memset(top->start, 0, size);
				return;
			}
			prev = q;
			q = q->prev;
		}
	}

	q = top;
	while (q != NULL) {
		if (q->start == ptr->start) {
			printf("\n%d \n", _g_bytes_allocated - ((int)q->start - (int)_g_allocator_memory));
			
			memcpy(buffer, prev->start, _g_bytes_allocated - ((int)q->start - (int)_g_allocator_memory));
			memcpy(q->start,buffer, _g_bytes_allocated - ((int)q->start - (int)_g_allocator_memory));
			tmp=q;
			while(tmp!=top){
			tmp->size=tmp->next->size;
			tmp=tmp->next;
			}
			tmp=q->next;
			while( tmp !=top)
			{	
				move+=tmp->size;
				tmp->start=q->start+move;
				tmp=tmp->next;
			}
			break;
		}
		prev = q;
		q = q->prev;
	}

	_g_bytes_allocated -= size;
	*error = M_ERR_OK;
}

void m_read(struct block read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {
	if (size_to_read > read_from_id.size) { *error = M_ERR_OUT_OF_BOUNDS; return; }
	if (&read_from_id != NULL && read_from_id.start != NULL)
	{
		memcpy(read_to_buffer, read_from_id.start, size_to_read);
	}
	else { *error = M_ERR_INVALID_CHUNK; }

	*error = M_ERR_OK;
}

void m_write(struct block write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
	if (&write_to_id == NULL) { *error = M_ERR_INVALID_CHUNK; }

	memcpy(write_to_id.start, write_from_buffer, size_to_write);
	if (size_to_write > write_to_id.size) { *error = M_ERR_OUT_OF_BOUNDS; return; }
	*error = M_ERR_OK;
}

void m_init(int number_of_pages, int size_of_page) {
	if (_g_allocator_memory != NULL) free(_g_allocator_memory);

	_g_allocator_memory_size = number_of_pages * size_of_page;
	_g_allocator_memory = (char*)malloc(_g_allocator_memory_size);
	_g_bytes_allocated = 0;
}
