#include <stdlib.h>
#include <string.h>

#include "memory.h"

char* _g_allocator_memory = NULL;
int _g_allocator_memory_size = 0;
int _g_bytes_allocated = 0;

struct block* begin = NULL;
struct block* top = NULL;

void m_init(int number_of_pages, int size_of_page)
{
	if (_g_allocator_memory != NULL) free(_g_allocator_memory);

	_g_allocator_memory_size = number_of_pages * size_of_page;
	_g_allocator_memory = malloc(_g_allocator_memory_size);
	_g_bytes_allocated = 0;
}

m_id m_malloc(int size_of_chunk, m_err_code* error_code)
{
	if (_g_bytes_allocated + size_of_chunk > _g_allocator_memory_size) {
		*error_code = M_ERR_ALLOCATION_OUT_OF_MEMORY;
		return NULL;
	}

	struct block* temp = malloc(sizeof(*temp));
	struct block* current = begin;
	bool result = false;

	if (begin != NULL) {
		result = insert_block(temp, current, size_of_chunk);
	} else {
		begin = temp;
		temp->memory = _g_allocator_memory + size_of_chunk;
	}

	if(!result) {
		temp->size = size_of_chunk;
		temp->prev = top;
		temp->next = NULL;
		temp->is_next_null = true;
		if (temp->prev != NULL) {
			temp->memory = (char*)top->memory + size_of_chunk;
			temp->prev->next = temp;
			temp->prev->is_next_null = false;
		}
		top = temp;		
	}

	*error_code = M_ERR_OK;
	return (m_id)temp;
}

void m_free(m_id ptr, m_err_code* error_code)
{
	if (ptr == NULL) {
		*error_code = M_ERR_ALREADY_DEALLOCATED;
		return;
	}

	struct block* chunk = ptr;
	struct block* temp;

	if (chunk->memory == top->memory) {
		temp = top;
		top = chunk->prev;
		chunk->prev->is_next_null = true;
		free(temp);
	} else if (chunk->memory == begin->memory) {
		temp = begin;
		begin = chunk->next;
		free(temp);
	} else {
		temp = chunk;
		chunk->prev->next = temp->next;
		chunk->next->prev = temp->prev;
		chunk->prev->is_next_null = true;
		free(chunk);
	}

	*error_code = M_ERR_OK;
	return;
}

void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error_code)
{
	struct block* read_from = read_from_id;

	if (size_to_read > read_from->size) {
		*error_code = M_ERR_OUT_OF_BOUNDS;
		return;
	}

	if (&read_from_id != NULL && read_from->memory != NULL) {
		memcpy(read_to_buffer, read_from->memory, size_to_read);
	} else {
		*error_code = M_ERR_INVALID_CHUNK;
		return;
	}

	*error_code = M_ERR_OK;
	return;
}

void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error_code)
{
	struct block* write_to = write_to_id;

	if (write_to_id == NULL) {
		*error_code = M_ERR_INVALID_CHUNK;
		return;
	}

	if (size_to_write > write_to->size) {
		*error_code = M_ERR_OUT_OF_BOUNDS;
		return;
	}

	memcpy(write_to->memory, write_from_buffer, size_to_write);

	*error_code = M_ERR_OK;
	return;
}

bool insert_block(struct block* temp, struct block* current, int size_of_chunk) {
	if ((char*)begin->memory - _g_allocator_memory >= size_of_chunk) {
		temp->size = size_of_chunk;
		temp->memory = _g_allocator_memory + size_of_chunk;
		temp->next = begin;
		temp->prev = NULL;

		begin->prev = temp;
		begin = temp;

		if ((char*)begin->memory - _g_allocator_memory == size_of_chunk) 
			temp->is_next_null = false;

		return true;
	}

	while (current != top) {
		if (current->is_next_null && current->next != NULL) {
			if ((int)current->next->memory - (int)current->memory - current->next->size >= size_of_chunk) {
				temp->memory = (char*)current->memory + size_of_chunk;
				temp->size = size_of_chunk;
				temp->next = current->next;
				temp->prev = current;

				current->is_next_null = false;
				current->next->prev = temp;
				current->next = temp;

				if ((int)current->next->memory - (int)current->memory - current->next->size == size_of_chunk)
					temp->is_next_null = false;

				return true;
			}
		}
		current = current->next;
	}
	return false;
}

