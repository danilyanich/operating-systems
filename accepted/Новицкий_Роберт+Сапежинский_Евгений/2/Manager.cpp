
#include <thread>
#include "Manager.h"

void Manager::run(Barber& barber)
{
    this->barber = &barber;
    std::vector<std::thread> threads;
    for (int i = 0; i < 5; i++){
        threads.emplace_back(std::thread(&Manager::createUser, this));
        threads.emplace_back(std::thread(&Manager::createUser, this));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        threads.emplace_back(std::thread(&Manager::createUser, this));
    }
    for (auto &a : threads)a.detach();
    std::this_thread::sleep_for(std::chrono::seconds(20));
    std::thread e(&Manager::createUser, this);
    e.join();
}

void Manager::createUser()
{
    User user;
    barber->getUser(user);
}
