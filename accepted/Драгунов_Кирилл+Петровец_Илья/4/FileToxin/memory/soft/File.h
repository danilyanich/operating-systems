//
// Created by Petri on 10/13/2020.
//

#ifndef FILESYSTEM_FILE_H
#define FILESYSTEM_FILE_H

#include <string>
#include "../hard/IndexesTable.h"

class File {
public:
    int id;
    std::string name;
    Index* first;

    File(int id, string name, Index *first);

    const string &getName() const;

    Index *getFirst() const;
};


#endif //FILESYSTEM_FILE_H
