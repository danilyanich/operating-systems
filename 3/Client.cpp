#pragma once
#pragma comment(lib, "Ws2_32.lib")

#include <winsock.h>
#include <string>
#include "Utils.h"
#include "Logger.cpp"

using namespace std;

class Client {
public:
    Client() {
        lengthOfLine = 10;
        generatedLine = generateLine(lengthOfLine);
        bool useConsoleForLogging = true;
        string pathToLogFile = getCurrentPath() + "\\logs\\logs" + generatedLine + ".log";
        logger = new Logger(pathToLogFile, useConsoleForLogging);
        createSocket();
    }

    ~Client() {
        WSACleanup();
        logger->info("Destroy client");
        delete logger;
    }

    bool connectToServer(const string &address, const u_short port) {
        sockaddr_in socketAddress{};
        socketAddress.sin_family = AF_INET;
        socketAddress.sin_addr.S_un.S_addr = inet_addr(address.c_str());
        socketAddress.sin_port = htons(port);
        if (connect(clientSocket, (sockaddr *) &socketAddress, sizeof(socketAddress)) != SOCKET_ERROR) {
            logger->info("Connect to server is successfully, ip:" + address + " , port:" + to_string(port));
            return true;
        } else {
            logger->error("Connect to server is failed, ip:" + address + " , port:" + to_string(port));
            return false;
        }
    }

    void sleep() {
        srand(time(nullptr));
        const int milliseconds = rand() % 9 + 2;
        int seconds = milliseconds * 1000;
        logger->info("Sleep " + to_string(seconds) + " seconds");
        Sleep(seconds);
    }

    void sendRequest() {
        int requestLength = lengthOfLine + 1;
        if (send(clientSocket, generatedLine.c_str(), requestLength, 0) != SOCKET_ERROR) {
            logger->info("Send line: " + generatedLine);
        } else {
            logger->warn("Sending line is failed, line: " + generatedLine);
        }
    }

    void receiveResponse() {
        string response;
        do {
            const int bufferSize = 1024;
            char buffer[bufferSize];
            if (recv(clientSocket, buffer, bufferSize, 0) > 0) {
                response.append(buffer);
            } else {
                break;
            }
        } while (true);
        logger->info("Receive data: " + response);
    }

    void disconnect() {
        if (closesocket(clientSocket) != SOCKET_ERROR) {
            logger->info("Disconnect socket successfully");
        } else {
            logger->error("Disconnect failed");
        }
    }

private:
    SOCKET clientSocket;

    Logger *logger;

    int lengthOfLine;

    string generatedLine;

    void createSocket() {
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket != INVALID_SOCKET) {
            logger->info("Create socket");
        } else {
            logger->error("Socket is invalid");
        }
    }
};