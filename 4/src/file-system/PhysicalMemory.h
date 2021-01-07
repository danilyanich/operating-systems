
#ifndef FILE_SYSTEM_PHYSICALMEMORY_H
#define FILE_SYSTEM_PHYSICALMEMORY_H

#include <vector>
#include "Block.h"


class PhysicalMemory {
public:
    PhysicalMemory(std::string &&mem_data) : data(mem_data) {}

    std::pair<size_t, size_t> Add(std::string &&str) {
        size_t len = str.size();
        data.insert(last_index * BLOCK_SIZE, str);
        size_t prev_index = last_index;
        last_index += (len + BLOCK_SIZE - 1) / BLOCK_SIZE;
        return {prev_index, last_index};
    }

    void Delete(size_t from, size_t to) {
        for (size_t i = from * BLOCK_SIZE; i < to * BLOCK_SIZE; ++i) {
            data[i] = ' ';
        }
    }

    std::string_view Read(size_t from, size_t to) const {
        return std::string_view(data).substr(from * BLOCK_SIZE, to * BLOCK_SIZE);
    }

    void Write(size_t from, size_t to, std::string &&data) {
        if (data.size() > (to - from) * BLOCK_SIZE) {
            throw std::invalid_argument("data size more then range");
        }
        size_t passed_data_idx = 0;
        for (size_t i = from * BLOCK_SIZE; i < to * BLOCK_SIZE && passed_data_idx < data.size(); ++i) {
            this->data[i] = data[passed_data_idx];
            ++passed_data_idx;
        }
        for (size_t i = from * BLOCK_SIZE + passed_data_idx; i < to * BLOCK_SIZE; ++i) {
            this->data[i] = ' ';
        }
    }

private:
    size_t last_index = 0;
    std::string data;
};


#endif //FILE_SYSTEM_PHYSICALMEMORY_H
