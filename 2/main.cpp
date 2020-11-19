#include <thread>
#include "Manager.h"

int main()
{
    Barber barber;
    Manager manager{};
    barber.init(3);
    std::thread barber_thread(&Barber::startWorking, &barber);
    std::thread manager_thread(&Manager::run, &manager, std::ref(barber));
    std::thread manager_threa(&Manager::run, &manager, std::ref(barber));
    barber_thread.join();
    return 0;
}
