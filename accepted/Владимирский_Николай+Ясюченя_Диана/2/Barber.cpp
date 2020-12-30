//
// Created by nicolas on 24/10/2020.
//
#include <unistd.h>
#include <iostream>
#include "Barber.h"

void Barber::init(int chairs)
{
    work_place.store(1);
    free_chairs.store(chairs);
}

void Barber::startWorking()
{
    while (work_place.load()){
        std::cout << "Barber:Sleeping\n";
        sleep(1);
    }
    work();
}

void Barber::work()
{
    while (!q.empty()){
        auto temp = q.front();
        q.pop();
        free_chairs++;
        for (int i = 0; i < 5; i++){
            temp.barbering();
            std::cout << "Barber:Working\n";
            sleep(1);
        }
        temp.stopBarbering();
    }
    work_place.store(1);
    startWorking();
}

void Barber::getUser(User user)
{
    queue_mutex.lock();
    if (work_place.load()){
        work_place--;
        q.push(user);
        free_chairs--;
    } else {
        if (free_chairs.load() == 0){
            user.leave();
        } else {
            free_chairs--;
            q.push(user);
            user.inQueue();
        }
    }
    queue_mutex.unlock();
}
