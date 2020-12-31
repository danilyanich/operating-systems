//
// Created by Anton Schur on 12/12/2020
//

#include "Table.h"

std::unique_ptr<Chunk>& Table::getChunk(int addres)
{
    return Table::table.at(addres);
}

int Table::createChunk()
{
    Table::table[indexes] = std::make_unique<Chunk>(indexes);
    return indexes++;
}

void Table::delChunk(int addres)
{
    auto index = Table::table.at(addres).get();
    if (!index->isLast)
    {
        delChunk(index->addres);
    }
    table.erase(addres);
}
