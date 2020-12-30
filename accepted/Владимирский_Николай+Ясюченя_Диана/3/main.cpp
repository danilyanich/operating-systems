#include "TCPSocket.h"
#include <iostream>
#include <chrono>
#include <future>

float calcFunction(uint16_t index);

int main()
{
    SocketAddress addres(44444, inet_addr("0.0.0.0"));
    auto client = TCPSocket::CreateTCP();
    if (client->Connect(addres) == NO_ERROR)
    {
        std::cout << "Connected\n";
        auto start_timer = std::chrono::steady_clock::now();
        while (std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - start_timer).count() < 100)
        {
            uint16_t index;
            char buffer[3];
			auto future = std::async(&TCPSocket::Recieve, client, buffer, 3);
            if (future.wait_for(std::chrono::seconds(2)) != std::future_status::timeout)
            {
                std::cout << "Recieved\n";
                index = *reinterpret_cast<uint16_t *>(&buffer[1]);

                if (buffer[0] == 'n')
                {
                    std::cout << "Disconnect";
                    break;
                }

                auto answer = calcFunction(index);
                if (client->Send((char *) &answer, 4) == 4)
                {
                    std::cout << "Sent\n";
                }
            } else
            {
                std::cout << "Disconnect";
                break;
            }
        }
    } else std::cout << "Cant connect\n";
    return 0;
}

float calcFunction(uint16_t index)
{
    return 0.f;
}
