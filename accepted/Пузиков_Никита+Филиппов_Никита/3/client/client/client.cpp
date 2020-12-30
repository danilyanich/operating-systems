#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <thread>

#pragma warning(disable:4996)

int main() {
    WSADATA wsa_data;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsa_data) != 0) {
        std::cout << "Error!\n";
        exit(1);
    }

    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(connection, (SOCKADDR*)&addr, sizeofaddr) != 0) {
        std::cout << "Error 2!\n";
        return 1;
    }

    std::cout << "COnnected!!\n";
    char msg[256];

    while (true) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        std::cin.getline(msg, sizeof(msg));
        send(connection, msg, sizeof(msg), NULL);

        Sleep(10);
    }

    system("pause");
    return 0;
}