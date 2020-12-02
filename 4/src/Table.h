//
// Created by Makarevich on 25/12/2020.
//
#pragma once

#ifndef OS4_TABLE_H
#define OS4_TABLE_H

#include <map>
#include <memory>
#include <cstring>

struct Chunk
{
    Chunk() = default;
    explicit Chunk(int addres) :
        addres(addres),
        copy(1),
        isLast(true)
    {

    }

    virtual ~Chunk()
    {
        if (deletable)
            delete memory;
    }

    void allocateMem(char* buff)
    {
        memory = new char[sizeof(buff)];
        std::strcpy(memory, buff);
        deletable = true;
    }

    int addres;
    int copy;
    bool isLast;
    char* memory;
private:
    bool deletable = false;
};

class Table
{
    static inline int indexes = 0;
    static inline std::map<int, std::unique_ptr<Chunk>> table{};
public:
    Table() = delete;

    static void delChunk(int addres);
    static std::unique_ptr<Chunk>& getChunk(int addres);
    static int createChunk();
};


#endif //OS4_TABLE_H
