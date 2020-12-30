
#pragma once

#ifndef OS2_USER_H
#define OS2_USER_H


#include <atomic>

class User
{
    inline static std::atomic_int index = 0;
    int ID;
public:
    User(){ID = index++;}
    void barbering() const;
    void stopBarbering() const;
    void leave() const;
    void inQueue() const;
};


#endif
