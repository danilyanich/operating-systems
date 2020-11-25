//
// Created by nicolas on 26/10/2020.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <future>
#include "TCPSocket.h"

void machineHandler(const TCPSocketPtr &client);

int writeFile(uint16_t index, float result);

std::atomic_uint16_t counter = 0;
std::atomic_uint16_t curr_counter = 0;
std::mutex counter_mutex;

int main()
{
    uint16_t a = 65535;std::cout << a << "\n";
    TCPSocketPtr server = TCPSocket::CreateTCP();
    SocketAddress address(44444, INADDR_ANY);
    if (server->Bind(address) != NO_ERROR)
    {
        return -1;
    }
    if (server->Listen() == NO_ERROR)
    {
        SocketAddress addr;
        while (true)
        {
            auto fut = std::async(&TCPSocket::Accept, server, std::ref(addr));
            while (fut.wait_for(std::chrono::seconds(1)) == std::future_status::timeout)
            {
                std::cout << curr_counter << "\n";
                if (curr_counter == 65535)abort();
            }
            auto client = fut.get();
            std::thread thr(machineHandler, client);
            thr.detach();
        }
    }
    return 0;
}

uint16_t getAddCounter()
{
    counter_mutex.lock();
    uint16_t temp = counter.load();
    if (temp != 65535)counter++;
    counter_mutex.unlock();
    return temp;
}

void machineHandler(const TCPSocketPtr &client)
{
    char buffer[3];
    buffer[0] = 'y';
    auto temp = getAddCounter();
    memcpy(buffer + 1, (char *) &temp, 2);
    while (client->Send(buffer, 3))
    {
        char rec_buffer[4];
        client->Recieve(rec_buffer, sizeof(float));
        float answer = *reinterpret_cast<float *>(&rec_buffer);

        int write_file_res = writeFile(temp, answer);
        while (write_file_res == 0 || write_file_res == -1)
        {
            if (write_file_res == -1)
            {
                buffer[0] = 'n';
                break;
            }
            write_file_res = writeFile(temp, answer);
        }
        temp = getAddCounter();
        memcpy(buffer + 1, (char *) &temp, 2);
    }
}

int writeFile(uint16_t index, float result)
{
    if (curr_counter == 65535)return -1;
    if (index != curr_counter) return 0;
    std::ofstream file("res.txt", std::ios::app);
    if (file.is_open())
    {
        file << index << " " << result << "\n";
    } else std::cout << "FILE PROBLEMS";
    file.close();
    curr_counter++;
    return 1;
}
