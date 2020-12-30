#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <thread>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
CRITICAL_SECTION section;
static std::vector<char*> strings(0);

void enable_sockets() {
    WSADATA wsaData;
    int iResult;
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
    }
}

int __cdecl main(void)
{
    FILE *f = fopen("log.txt", "ab+");
    enable_sockets();
    u_long iMode = 1;
    int iResult;
    auto ListenSocket = INVALID_SOCKET;
    auto ClientSocket = INVALID_SOCKET;
    struct addrinfo *result = nullptr;
    struct addrinfo hints{};
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
    if ( iResult != 0 ) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    iResult = ioctlsocket(ListenSocket, FIONBIO, &iMode);
    if (iResult != NO_ERROR) {
        perror("ioctl() failed");
        printf("%d", iResult);
        closesocket(ListenSocket);
        exit(-1);
    }
    freeaddrinfo(result);
    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    while (true) {
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
//            char sub[15];
//            sprintf(sub, "[%d]: idle\n",
//                    std::this_thread::get_id());
//            fprintf(f, "%s", sub);
//            printf("%s", sub);
            continue;
        } else {
            int rc;
            char sub[15];
            sprintf(sub, "[%d]: idle\n",
                   std::this_thread::get_id());
            fprintf(f, "%s", sub);
            printf("%s", sub);
//            strings.push_back(sub);
            retry:
            char buffer[1024];
            rc = recv(ClientSocket, buffer, sizeof(buffer), 0);
            if (rc < 0) {
                if (errno != EWOULDBLOCK) {
                    printf("Thread %d establishing connection\n",
                            std::this_thread::get_id());
                    goto retry;
                }
            } else if (rc == 0) {
                printf("Connection closed\n");
                char cls[15];
                sprintf(cls, "[%d] client %d disconnected",
                        std::this_thread::get_id(),
                        ClientSocket);
                fprintf(f, "%s", cls);
            } if (rc > 0) {
                int len = rc;
                printf("  %d bytes received\n", len);
                strings.push_back(buffer);
                fprintf(f, "%.*s\n", len, buffer);
                snd:
                rc = send(ClientSocket, buffer, len, 0);
                if (rc != len) {
                    goto snd;
                }
            } else {
                goto retry;
            }
        }
    }
//    closesocket(ClientSocket);
//    WSACleanup();

    return 0;
}