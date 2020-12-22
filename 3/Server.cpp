#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock.h>
#include <vector>
#include <map>
#include "ClientDetails.cpp"
#include <process.h>
#include "Logger.cpp"

#pragma comment(lib, "Ws2_32.lib")
#ifndef  WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "Utils.h"

using namespace std;

class Server {
public:
    Server() {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        mainHandle = getCurrentThreadHandle();
        string pathToLogFile = getCurrentPath() + "\\logs.log";
        bool useConsoleForLogginng = true;
        logger = new Logger(pathToLogFile, useConsoleForLogginng);
        isActive = true;
        createSocket();
        InitializeCriticalSection(&bufferSection);
    }

    ~Server() {
        WaitForSingleObject(backgroundHandle, INFINITE);
        CloseHandle(backgroundHandle);
        DeleteCriticalSection(&bufferSection);
        WSACleanup();
        logger->info("Destroy server");
        delete logger;
    }

    void launch(const string &ipAddress, const u_short ipPort) {
        if (!bindSocket(ipAddress, ipPort)) {
            return;
        }
        string message = getThreadHandle() + " : server is running on " + getServerAddress();
        updateBuffer(message);
        logger->info(message);
        if (!configureListening()) {
            return;
        }
        startBackgroundTask();
        run();
    }

private:

    Logger *logger;

    bool isActive;

    string mainHandle;

    HANDLE backgroundHandle;

    unsigned backgroundThreadId;

    SOCKET serverSocket;

    CRITICAL_SECTION bufferSection;

    vector<string> messagesBuffer;

    map<SOCKET, ClientDetails> socketToClientDetails;

    void createSocket() {
        serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serverSocket != INVALID_SOCKET) {
            bool enable = true;
            ioctlsocket(serverSocket, FIONBIO, (u_long *) &enable);
            logger->info("Socket is created successfully");
        } else {
            logger->error("Socket creation is failed");
        }
    }

    bool bindSocket(const string &ipAddress, const u_short port) {
        sockaddr_in address{};
        address.sin_family = AF_INET;
        address.sin_addr.S_un.S_addr = inet_addr(ipAddress.c_str());
        address.sin_port = htons(port);
        if (bind(serverSocket, (sockaddr *) &address, sizeof(address)) != SOCKET_ERROR) {
            logger->info("Bind socket successfully on address: " + ipAddress + " and port: " + to_string(port));
            return true;
        } else {
            logger->error("Bind socket is failed on address: " + ipAddress + " and port: " + to_string(port));
            return false;
        }
    }

    string getThreadHandle() {
        return "[" + mainHandle + "]";
    }

    string getServerAddress() {
        sockaddr address{};
        int addressSize = sizeof(address);
        getsockname(serverSocket, &address, &addressSize);
        return getSocketAddress(&address);
    }

    string getSocketAddress(const sockaddr *sockaddr) {
        const auto *sock_in = (const sockaddr_in *) sockaddr;
        return string("address:") + inet_ntoa(sock_in->sin_addr) +
               string(", port:") + to_string(ntohs(sock_in->sin_port));
    }

    void updateBuffer(const string &message) {
        EnterCriticalSection(&bufferSection);
        messagesBuffer.push_back(message);
        LeaveCriticalSection(&bufferSection);
    }

    bool configureListening() {
        if (listen(serverSocket, SOMAXCONN) != SOCKET_ERROR) {
            logger->info("Listening is run successfully");
            return true;
        } else {
            logger->info("Listening is failed");
            return false;
        }
    }

    void startBackgroundTask() {
        backgroundHandle = (HANDLE) _beginthreadex(nullptr, 0, &Server::backgroundTaskWrapper,
                                                   (PVOID) this, 0, &backgroundThreadId);
    }

    static unsigned __stdcall backgroundTaskWrapper(PVOID param) {
        auto *server = (Server *) param;
        server->doBackgroundTask();
        return 0;
    }

    void doBackgroundTask() {
        logger->info("Background task is run successfully");
        stringstream backgroundThreadHandle;
        backgroundThreadHandle << backgroundHandle;
        string threadId = "[" + backgroundThreadHandle.str() + "]";
        while (isActive) {
            string message = threadId + " : idle";
            updateBuffer(message);
            logger->info(message);
            int second = 1000;
            Sleep(second);
        }
    }

    void run() {
        logger->info("Server is run...");
        do {
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(serverSocket, &readSet);
            timeval timeout = getTimeVal();
            if (select(0, &readSet, nullptr, nullptr, &timeout) > 0 && FD_ISSET(serverSocket, &readSet)) {
                acceptClient();
            }
            handleCurrentConnections();
        } while (isActive);
    }

    timeval getTimeVal() {
        timeval timeout{};
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        return timeout;
    }

    void acceptClient() {
        sockaddr clientAddress{};
        int addressSize = sizeof(clientAddress);
        SOCKET clientSocket = accept(serverSocket, &clientAddress, &addressSize);
        if (clientSocket != INVALID_SOCKET) {
            string message = getThreadHandle() + " : accept new client " + getSocketAddress(&clientAddress);
            updateBuffer(message);
            logger->info(message);
            bool enableNonBlockingMode = true;
            ioctlsocket(clientSocket, FIONBIO, (u_long *) &enableNonBlockingMode);
            socketToClientDetails.insert({clientSocket, ClientDetails()});
        } else {
            logger->warn("Accepting is failed, address of client: " + getSocketAddress(&clientAddress));
            return;
        }
    }

    void handleCurrentConnections() {
        if (socketToClientDetails.empty()) {
            return;
        }
        fd_set readSet, writeSet;
        configureSet(readSet);
        configureSet(writeSet);
        timeval timeout = getTimeVal();
        if (select(0, &readSet, &writeSet, nullptr, &timeout) > 0) {
            for (auto &socketToClientDetail : socketToClientDetails) {
                SOCKET clientSocket = socketToClientDetail.first;
                if (FD_ISSET(clientSocket, &readSet)) {
                    receiveRequest(clientSocket);
                } else {
                    if (FD_ISSET(clientSocket, &writeSet) && socketToClientDetail.second.isMessageReceived()
                        && !socketToClientDetail.second.isMessageSent()) {
                        sendResponse(clientSocket);
                    }
                }
            }
        }
        removeDisconnectedClients();
    }

    void configureSet(fd_set &set) const {
        FD_ZERO(&set);
        for (auto &socketToClientDetail : socketToClientDetails) {
            FD_SET(socketToClientDetail.first, &set);
        }
    }

    void receiveRequest(SOCKET clientSocket) {
        const int bufferSize = 1024;
        char buffer[bufferSize] = "";
        if (recv(clientSocket, buffer, bufferSize, 0) > 0) {
            ClientDetails &client = socketToClientDetails.at(clientSocket);
            client.setMessage(string(buffer));
            client.setMessageReceived(true);
            string message = getThreadHandle() + " : received message from " + getClientAddress(clientSocket)
                             + ": \"" + client.getMessage() + "\"";
            updateBuffer(message);
            logger->info(message);
        } else {
            markAsDisconnected(clientSocket);
        }
    }

    string getClientAddress(SOCKET clientSocket) {
        sockaddr clientAddress{};
        int addressSize = sizeof(clientAddress);
        getpeername(clientSocket, &clientAddress, &addressSize);
        return getSocketAddress(&clientAddress);
    }

    void markAsDisconnected(SOCKET clientSocket) {
        string clientAddress = getClientAddress(clientSocket);
        if (closesocket(clientSocket) != SOCKET_ERROR) {
            socketToClientDetails.at(clientSocket).setDisconnected(true);
            string message = getThreadHandle() + " : client " + clientAddress + " is disconnected successfully";
            updateBuffer(message);
            logger->info(message);
        } else {
            logger->warn("Disconnection of client: " + clientAddress + " is failed");
            return;
        }
    }

    void sendResponse(SOCKET clientSocket) {
        ClientDetails &clientDetails = socketToClientDetails.at(clientSocket);
        const char *buffer = clientDetails.getMessage().c_str();
        const int lengthOfBuffer = clientDetails.getMessage().size() + 1;
        const int leftBytesToSend = lengthOfBuffer - clientDetails.getPosition();
        int bytesSent = send(clientSocket, buffer + clientDetails.getPosition(), leftBytesToSend, 0);
        if (clientDetails.getPosition() + bytesSent >= clientDetails.getMessage().size()) {
            clientDetails.setMessageSent(true);
            string message = getThreadHandle() + " : send message " +
                             clientDetails.getMessage() + " to " + getClientAddress(clientSocket);
            logger->info(message);
            shutdown(clientSocket, 1);
        } else {
            clientDetails.setPosition(clientDetails.getPosition() + bytesSent);
        }
    }

    void removeDisconnectedClients() {
        for (auto iterator = socketToClientDetails.begin(); iterator != socketToClientDetails.end();) {
            if (iterator->second.isDisconnected()) {
                iterator = socketToClientDetails.erase(iterator);
            } else {
                iterator++;
            }
        }
    }
};