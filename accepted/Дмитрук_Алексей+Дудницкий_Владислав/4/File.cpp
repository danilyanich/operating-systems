#include "File.h"

void File::write(char* buf) const
{
    auto first = Table::getChunk(index).get();
    while (!first->isLast)
    {
        first = Table::getChunk(first->addres).get();
    }
    first->isLast = false;
    first->addres = Table::createChunk();
    Table::getChunk(first->addres)->allocateMem(buf);
}

std::string File::read() const
{
    std::string res;
    auto cur = Table::getChunk(index).get();
    while (!cur->isLast)
    {
        if (cur->memory)
            res.append(cur->memory);
        cur = Table::getChunk(cur->addres).get();
    }
    res.append(cur->memory);
    return res;
}