#pragma comment(lib, "Ws2_32.lib")

#define BUFFER_SIZE 8192
#define OK_CODE 200
#define REDIRECT_CODE 300
#define BAD_REQUEST_CODE 400
#define SERVER_ERROR_CODE 500

#include<iostream>
#include<string>
#include <winsock.h>
#include <fstream>
#include <regex>
#include "ImageData.cpp"
#include "Logger.cpp"
#include "Utils.cpp"

using namespace std;

class ImageDownloader {

public:

    ImageDownloader() {
        WSADATA lpWSAData;
        WSAStartup(MAKEWORD(1, 1), &lpWSAData);
        logger = new Logger(Utils::getCurrentPath() + "\\logs.log");
        logger->info("Downloader started");
    }

    ~ImageDownloader() {
        WSACleanup();
        logger->info("Downloader stopped");
        logger->close();
        delete logger;
    }

    void download(ImageData *data) {
        string url = data->getUrl();
        if (!Utils::isValidUrl(url)) {
            logger->error("Url " + url + " is invalid");
            return;
        }
        string path = data->getPath();
        SOCKET socket = connectToHost(url);
        sendRequest(socket, url);
        char *content = readContent(socket, url);
        int statusCode = getStatusCode(content);
        if (statusCode == OK_CODE) {
            logger->info("Status code: " + to_string(statusCode));
            char *body = getBody(content);
            save(path, body, findContentLength(content));
        } else {
            logger->error("Status code: " + to_string(statusCode));
        }
        stop(socket);
        delete[] content;
    }

private:

    Logger *logger;

    SOCKET connectToHost(const string &url) {
        sockaddr_in socketAddress = getSockaddr_in(url);
        SOCKET socketObject = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        connect(socketObject, (sockaddr *) &socketAddress, sizeof(socketAddress));
        logger->info("Connected to host by url " + url);
        return socketObject;
    }

    sockaddr_in getSockaddr_in(const string &url) {
        hostent *hostent = gethostbyname(getHostFromUrl(url).c_str());
        sockaddr_in socketAddress{};
        socketAddress.sin_family = AF_INET;
        socketAddress.sin_addr.S_un.S_un_b.s_b1 = hostent->h_addr[0];
        socketAddress.sin_addr.S_un.S_un_b.s_b2 = hostent->h_addr[1];
        socketAddress.sin_addr.S_un.S_un_b.s_b3 = hostent->h_addr[2];
        socketAddress.sin_addr.S_un.S_un_b.s_b4 = hostent->h_addr[3];
        socketAddress.sin_port = htons(80);
        return socketAddress;
    }

    string getHostFromUrl(const string &url) {
        regex hostPattern("^.*://([^/?:]+)/?.*$");
        return regex_replace(url, hostPattern, "$1");
    }

    string getPathToImageFromUrl(const string &url) {
        regex pathPattern("^.*://([^/?:]+)(/?.*$)");
        return regex_replace(url, pathPattern, "$2");
    }

    void sendRequest(SOCKET socket, const string &url) {
        char request[1024];
        strcpy(request, "GET ");
        strcat(request, getPathToImageFromUrl(url).c_str());
        strcat(request, " HTTP/1.1\r\n");
        strcat(request, "Host: ");
        strcat(request, getHostFromUrl(url).c_str());
        strcat(request, "\r\nConnection: close");
        strcat(request, "\n\n");
        logger->info("Created request for " + url);
        send(socket, request, strlen(request), 0);
        logger->info("Sent request to " + url);
    }

    void stop(SOCKET socket) {
        closesocket(socket);
    }

    char *readContent(SOCKET socket, const string &url) {
        int size = 1024 * 1024 * 20;
        char *result = new char[size];
        strcpy(result, "");
        char *buffer = result;
        while (true) {
            int content = recv(socket, buffer, BUFFER_SIZE, 0);
            if (content > 0) {
                buffer += content;
                logger->info("Received " + to_string(content) + " bytes from " + url);
            } else {
                break;
            }
        }
        return result;
    }

    int findContentLength(char *content) {
        int offset = strlen("Content-Length: ");
        return atoi(strstr(content, "Content-Length: ") + offset);
    }

    int getStatusCode(char *content) {
        int offset = strlen("HTTP/1.1");
        int statusCode = atoi(strstr(content, "HTTP/1.1 ") + offset);
        return Utils::normalizeStatusCode(statusCode);
    }

    char *getBody(char *content) {
        int offset = strlen("\r\n\r\n");
        return strstr(content, "\r\n\r\n") + offset;
    }

    void save(const string &path, char *data, streamsize length) {
        ofstream binFile;
        binFile.open(path, ios::out | ios::binary);
        binFile.write(data, length);
        binFile.close();
        logger->info("Saved image to " + path);
        logger->info("Size of image " + path + " is " + to_string(length) + " bytes");
    }
};