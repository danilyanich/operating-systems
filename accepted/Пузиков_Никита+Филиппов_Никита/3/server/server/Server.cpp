#pragma comment(lib, "ws2_32.lib")

#include <WinSock2.h>
#include <iostream>
#include <windows.h>
#include <thread>
#include "windows.h"
#include <fstream>

#pragma warning(disable:4996)

HANDLE mut;
LPCWSTR str = L"HELLO";
DWORD result;

SOCKET Connections[100];

void save_to_file(char msg[]) {
    result = WaitForSingleObject(mut, INFINITE);

    if (result == WAIT_OBJECT_0) {
        std::ofstream fout;
        fout.open("file_dump.txt", std::ios::app);

        std::cout << "wait\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));

        fout << msg << std::endl;
        fout.close();

        ReleaseMutex(mut);
    }
}

void print_(int index) {
    char msg[256];
    while (true) {
        recv(Connections[index], msg, sizeof(msg), NULL);
        save_to_file(msg);
    }
}

int main() {
    mut = CreateMutex(NULL, FALSE, str);

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

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 100; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

        if (newConnection == 0) {
            std::cout << "Error2!\n";
        }
        else {
            char msg[256];
            std::cout << "Create new connection!\n";
            Connections[i] = newConnection;

            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)print_, (LPVOID)i, NULL, NULL);
        }
    }


    system("pause");

    CloseHandle(mut);

    return 0;

}
