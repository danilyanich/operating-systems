
#pragma once

#ifndef OS2_BARBER_H
#define OS2_BARBER_H


#include <queue>
#include <mutex>
#include "User.h"

class Barber
{
public:
    void init(int chairs);
    void startWorking();
    void work();
    void getUser(User user);


    std::atomic_int work_place;
    std::atomic_int free_chairs;
private:
    mutable std::mutex queue_mutex;
    std::queue<User> q;
};


#endif
