//ignoring errors
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_NO_VA_START_VALIDATION

#pragma comment(lib, "ws2_32.lib")
//global libraries
#include <iostream>
#include <fstream>
#include <string>
#include <winsock2.h>
#include <thread>
#include <vector>

//local libraries
#include "Queue.h"
#include "Link.h"
#include "ImageServiceClass.h"
using namespace std;

fd_set readfds;
char logFileName[80];
CRITICAL_SECTION console;
CRITICAL_SECTION file;

unsigned __stdcall runThreadPool(void* pArg);
unsigned __stdcall downloadImage(void* pArg);


int main() {
    bool workMode = true;
    int choose;
    while (workMode == true) {
        ImageServiceClass imageService;
        printf("The program`s now start working\n");
        time_t t = time(0);
        struct tm* now = localtime(&t);
        strftime(logFileName, 80, "%Y-%m-%d %H.%M.log", now);
        CreateDirectoryA("./image/", 0);
        InitializeCriticalSection(&console);
        InitializeCriticalSection(&file);
        unsigned int qThreadID = 0;
        HANDLE threadsHandler = (HANDLE)_beginthreadex(NULL, 0, runThreadPool, NULL, 0, &qThreadID);
        cout << "Select the operating mode\n1-Main work\n2-Test work" <<  endl;
        cin >> choose;
        if (choose == 2) {
            imageService.runWorkTest();
            workMode = false;
        }
        else if (choose == 1)
        {
            imageService.start();
            workMode = false;
        }
        else {
            printf("Repeat operation");
        }
    }
    return 0;
}

void ImageServiceClass::start()
{
    string url;
    int numLink = -1;
    while (true) {
        cin >> url;
        startDownoloadProcess(&url, &numLink);
    }
}

void ImageServiceClass::runWorkTest()
{
    string urlAdress;
    char link[500];
    int numLink = -1;
    ifstream file("ImageTest/Test1.txt");
    file >> link;
    printf("%s", link);
    getline(file, urlAdress);
    startDownoloadProcess(&urlAdress, &numLink);
    file.close();
}

void ImageServiceClass::startDownoloadProcess(string* url, int* numlink)
{
    Queue queue(5);
    SOCKET socketArr[1024];
    string imageNameArr[1024];
    Link* link = convertToImageLink(*url, &socketAddressNumber, imageNameArr);
    socketArr[socketAddressNumber] = establishConnection(&link->hostName, &link->path);
    FD_SET(socketArr[socketAddressNumber], &readfds);
    (*numlink)++;
    Queue::Element element = { (*numlink), (*numlink) };
    bool rez = queue.put(&element, 1000);
    if (!rez) {
        logInfo("Failed to add new element to sync queue");
    }
}


SOCKET ImageServiceClass::establishConnection(string* host, string* path)
{
    WSADATA wd;
    SOCKET sock;
    struct hostent* hostInfo;

    int result = WSAStartup(MAKEWORD(2, 2), &wd);
    if (result != 0) {
        logInfo("Can't load WSDATA library");
        return result;
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        logInfo("Can't initialize socket");
        return sock;
    }

    sockaddr_in sai = { AF_INET };
    sai.sin_family = AF_INET;

    hostInfo = gethostbyname((*host).c_str());
    if (hostInfo == NULL) {
        logInfo("Incorrect link was input");
        return sock;
    }
    sai.sin_port = htons(80);
    memcpy(&sai.sin_addr, hostInfo->h_addr, 4);

    int check = connect(sock, (sockaddr*)&sai, sizeof(sai));
    if (check == SOCKET_ERROR) {
        logInfo("Can't establish connection with socket");
        return sock;
    }

    char* requestMessage = new char[sizeof(path) + sizeof(host) + REQUEST_MESSAGE_LENGTH];
    sprintf(requestMessage, "GET %s HTTP/1.1\r\nHost: %s\r\nConnection:Close\r\n\r\n", (*path).c_str(), (*host).c_str());
    int sendInfo = send(sock, requestMessage, strlen(requestMessage), 0);
    if (SOCKET_ERROR == sendInfo) {
        logInfo("Error occurred during send request");
        closesocket(sock);
        return sock;
    }
    delete[] requestMessage;
    return sock;
}

string ImageServiceClass::getCurrentTimeString()
{
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    char timeForAnswer[100];
    strftime(timeForAnswer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
    return timeForAnswer;
}

string ImageServiceClass::generateMessageOfImageDownoloading(string image, int bites)
{
    string timeForAnswer = getCurrentTimeString();
    char answer_char[200];
    string answer;

    if (bites > 0) {
        char message[1000];
        sprintf(message, "%s[INFO] - Image: %s is still downloading , Bites received: %d", timeForAnswer.c_str(), image.c_str(), bites);
        answer = message;
    }
    else if (bites < 0) {
        char message[1000];
        sprintf(message, "%s[INFO] - Image: %s don't downoload due to some problems", timeForAnswer.c_str(), image.c_str());
        answer = message;
    }
    else {
        char message[1000];
        sprintf(message, "%s[INFO] - Image: %s downoload successfully", timeForAnswer.c_str(), image.c_str());
        answer = message;
    }

    for (int j = 0; j < (int) answer.length(); j++) {
        answer_char[j] = answer[j];
    }

    answer_char[answer.length()] = '\0';
    return answer_char;
}

void ImageServiceClass::writeToConsole(CRITICAL_SECTION* section, string message, ostream* target)
{
    EnterCriticalSection(section);
    *target << message << endl;
    LeaveCriticalSection(section);
}

void ImageServiceClass::writeToLogFile(string stringToWrite)
{
    EnterCriticalSection(&file);
    FILE* fileToSave = fopen(logFileName, "a");
    fprintf(fileToSave, "%s\n", stringToWrite.c_str());
    fflush(fileToSave);
    fclose(fileToSave);
    LeaveCriticalSection(&file);
}

void ImageServiceClass::logInfo(const char* messageToWrite)
{
    writeToLogFile(messageToWrite);
    writeToConsole(&console, messageToWrite, &cout);
}

string ImageServiceClass::format(const string& format, ...)
{
    va_list args;
    va_start(args, format);
    size_t len = std::vsnprintf(NULL, 0, format.c_str(), args);
    va_end(args);
    std::vector<char> vec(len + 1);
    va_start(args, format);
    std::vsnprintf(&vec[0], len + 1, format.c_str(), args);
    va_end(args);
    return &vec[0];
}

unsigned __stdcall runThreadPool(void* pArg)
{
    ImageServiceClass imageService;
    Queue queue(5);
    while (true)
    {
        Queue::Element element = {};
        if (queue.pull(&element)) {
            unsigned int* arg = (unsigned int*)malloc(sizeof(unsigned int));
            if (arg) {
                *arg = element.numberLink;
                HANDLE handle = (HANDLE)_beginthreadex(NULL, 1000, downloadImage, (void*)arg, 0, &element.numberLink);
                imageService.activeHandles.push_back(handle);
            }
        }
    }
    return 0;
}

unsigned __stdcall downloadImage(void* pArg)
{
    ImageServiceClass imageService;
    SOCKET socketArr[1024];
    fstream image[1024];
    string imageNameArr[1024];
    unsigned int numPtr = *((int*)pArg);
    unsigned int numLink = numPtr;
    string timeForAnswer = imageService.getCurrentTimeString();
    int threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
    string message = imageService.format("%s [INFO] - Thread: %d start downnoload image %s", timeForAnswer.c_str(), threadId, imageNameArr[numLink].c_str());
    imageService.logInfo(message.c_str());
    char buf[10240];
    image[numLink].open("./image/" + imageNameArr[numLink], ios::out | ios::binary);
    memset(buf, 0, sizeof(buf));
    int n = recv(socketArr[numLink], buf, sizeof(buf) - 1, 0);
    if (n == -1) {
        imageService.logInfo("Failed to read socket data");
        free(pArg);
        _endthreadex(0);
        return 0;
    }
    else {
        char* cpos = strstr(buf, "\r\n\r\n");
        image[numLink].write(cpos + strlen("\r\n\r\n"), static_cast<unsigned __int64> (n) - (cpos - buf) - strlen("\r\n\r\n"));
        string message = imageService.generateMessageOfImageDownoloading(imageNameArr[numLink], n);
        imageService.writeToLogFile(message);
    }

    while (true) {
        if (FD_ISSET(socketArr[numLink], &readfds)) {
            memset(buf, 0, sizeof(buf));
            n = recv(socketArr[numLink], buf, sizeof(buf) - 1, 0);
            if (n > 0) {
                image[numLink].write(buf, n);
                string message = imageService.generateMessageOfImageDownoloading(imageNameArr[numLink], n);
                imageService.writeToLogFile(message);
            }
            else if (n == 0) {
                FD_CLR(socketArr[numLink], &readfds);
                image[numLink].close();
                string message = imageService.generateMessageOfImageDownoloading(imageNameArr[numLink], n);
                imageService.logInfo(message.c_str());
                break;
            }
            else if (n == -1) {
                imageService.logInfo("Failed to read socket data");
            }
        }
    }
    free(pArg);
    _endthreadex(0);
    return(0);
}