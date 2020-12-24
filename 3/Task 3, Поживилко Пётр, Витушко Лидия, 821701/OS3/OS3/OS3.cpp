#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <mutex>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <thread>
#include <string>
#include <fstream>  

//#include "client.cpp"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
CRITICAL_SECTION section;
using namespace std;

mutex mutex1;
 std::vector<string> strings;

void enable_sockets() {
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
    }
}

void SaveBuffer() {
    cout << "log.txt" << endl;
    FILE* f;
    fopen_s(&f, "log.txt", "ab+");
    mutex1.lock();
    //FILE* f = fopen_s("log.txt", "ab+");
    std::string s = "hello world\n";
    std::ofstream os("filename.txt");
    if (!os) { std::cerr << "Error writing to ..." << std::endl; }
    else {
        os << s;
    }
    for (size_t i = 0; i < strings.size(); i++)
    {
        //cout << "save::" << strings.at(i) << endl;
        os << strings[i]<<endl;
        //fprintf(f, "%s", strings.at(i));
       // fputs(strings.at(i), f);
    }
    mutex1.unlock();
    fclose(f);

    strings.clear();
}

void timer(int id) {
    char sub[55];
    cout << "timer++" << endl;
    while (true) {
        Sleep(1000);
        sprintf_s(sub, "[%d]: idle\n", std::this_thread::get_id());
        mutex1.lock();
        strings.emplace_back(sub);
        mutex1.unlock();
        cout << sub;
    }
    cout << "end timer" << endl;
}

void serverThread(SOCKET ClientSocket) {
    cout << "new thread: " << std::this_thread::get_id() << endl;
    //FILE* f = fopen("log.txt", "w+");
    bool active = true;
    std::thread timerThread(timer, 123);
    timerThread.detach();
    float retryTime = 0;
    char sub[35];
    sprintf_s(sub, "[%d]: accept new client %d\n", std::this_thread::get_id(), 20170);
    mutex1.lock();
    strings.emplace_back(sub);
    mutex1.unlock();
    cout << sub;
    //fprintf(f, "%s", sub);

    //sprintf_s(sub, "[%d]: idle\n", std::this_thread::get_id());
   // strings.emplace_back(sub);
    //fprintf(f, "%s", sub);

    //
    //printf("%s", sub);
    int rc;
    char buffer[1024];
    rc = recv(ClientSocket, buffer, sizeof(buffer), 0);
    cout << "rc=" << rc << endl;
    while (rc < 0) {
        rc = recv(ClientSocket, buffer, sizeof(buffer), 0);
    }
    if (rc == 0) {
        printf("Connection closed\n");
        char cls[55];

        sprintf_s(cls, "[%d] client %d disconnected", std::this_thread::get_id(), ClientSocket);
        mutex1.lock();
        strings.emplace_back(cls);
        mutex1.unlock();
        //fprintf(f, "%s", cls);
    }

    if (rc > 0) {
        int len = rc;
        printf("  %d bytes received\n", len);
        // cout << buffer<<endl;

        string s;
        s = buffer;
        s = s.erase(len, s.size());
        char* chr = &s[0];

        cout << "msg = " << chr << endl;
        mutex1.lock();
        //strings.emplace_back(chr);
        strings.emplace_back(s);
        //strings.emplace_back(&"------");
        mutex1.unlock();
        //fprintf(f, "%s", buffer);

        if (buffer[0] == 'I' && buffer[1] == 'N' && buffer[2] == 'T') {
            cout << "Saving buffer" << endl;
            SaveBuffer();
        }

        rc = send(ClientSocket, buffer, len, 0);

        active = false;
    }
}

void startServer() {
    enable_sockets();
    u_long iMode = 1;
    int iResult;
    auto ListenSocket = INVALID_SOCKET;
    auto ClientSocket = INVALID_SOCKET;
    struct addrinfo* result = nullptr;
    struct addrinfo hints {};
    int iSendResult;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;
    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);
    freeaddrinfo(result);
    iResult = listen(ListenSocket, SOMAXCONN);

    float idleTime = 0;
    cout << "startServer on thread " << std::this_thread::get_id();

    while (true) {
        ClientSocket = accept(ListenSocket, NULL, NULL);

        if (ClientSocket == INVALID_SOCKET) {
            if (idleTime > 350000) {
                idleTime = 0; //cout << "idle" << endl;
            }
            else {
                idleTime++;
            }
            continue;
        }
        else {
            //serverThread(ClientSocket);
            thread thread1(serverThread, (ClientSocket));
            thread1.join();
        }
    }
}

int __cdecl main(void)
{
    startServer();
    //thread thread11223(startServer);
    //    closesocket(ClientSocket);
    //    WSACleanup();

    return 0;
}