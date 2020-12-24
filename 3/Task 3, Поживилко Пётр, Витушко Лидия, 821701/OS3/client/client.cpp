
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <thread>
#include <chrono>
#include <iostream>



// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

int emulate_client(const char* sendbuf, int seconds) {
    std::cout << std::this_thread::get_id() << std::endl;
    int iResult;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    char recvbuf[DEFAULT_BUFLEN];
    int recvbuflen = DEFAULT_BUFLEN;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %d\n", iResult);

    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }
    iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
    closesocket(ConnectSocket);
    if (iResult > 0) {
        printf("Bytes received: %d\n", iResult);
    }
    return 0;
}

int __cdecl main(int argc, char** argv) {
    WSADATA wsaData;

    int iResult;
    int a;
    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }
    //    for(int i = 0; i < 10; i++) {
    //        //std::thread thread1(emulate_client, "yes_fgtgrgrguture\0", 2);
    //
    //    }
   // emulate_client("random", 2);
    emulate_client("1234567890\0", 1);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    emulate_client("xdhfjchkgvjl\0", 2);    
    std::this_thread::sleep_for(std::chrono::seconds(3));
    emulate_client("INT\0", 1); 
    std::this_thread::sleep_for(std::chrono::seconds(2));
    emulate_client("another_record\0", 3);
    //std::thread thread1(emulate_client, "yes_fgtgrgrguture\0", 2);
    //std::thread thread2(emulate_client, "no_fgtrggrtgtgtrgtuture\0", 3);
    // cleanup
    WSACleanup();
    return 0;
}