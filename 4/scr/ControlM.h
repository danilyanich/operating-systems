#ifndef OS4_CONTROLM_H
#define OS4_CONTROLM_H
#include "PhMemory.h"
#include <string>

using namespace std;

class ControlM {
public:
    PhMemory memory;
    explicit ControlM(PhMemory memory);
    Block* allocateFile();
    bool appendValueTo(Block* first, char v);
    Block* findFreeIdx();
    string readFile(Block* initial) const;
    Block* copyFile(Block* idx);
    static void deleteFile(Block* idx);
};


#endif //OS4_CONTROLM_H
