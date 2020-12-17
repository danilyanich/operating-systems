#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <thread>
#include <string>

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
    for (int n = 0; n < 5; n++) {
        int a = rand();
        int b = rand();
        int c = a + b;
        std::string rel = std::to_string(c);
        for (int i = 0; i < rel.size(); i++) {
            msg[i] = rel[i];
        }
        for (int i = rel.size(); i < 256; i++) {
            char v = char(" ");
            msg[i] = v;
        }
        send(connection, msg, sizeof(msg), NULL);

        Sleep(10);
    }

    system("pause");
    return 0;
}