
#ifndef OS2_MANAGER_H
#define OS2_MANAGER_H

#include "Barber.h"
#include <chrono>

class Manager
{
public:
    void run(Barber& barber);
    void createUser();


private:
    Barber* barber = nullptr;
};


#endif
