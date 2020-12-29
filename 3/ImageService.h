//
// Created by alexandr on 29.12.20.
//

#ifndef INC_3_IMAGESERVICE_H
#define INC_3_IMAGESERVICE_H

class ImageServiceClass {

public:
    int socketAddressNumber = -1;
    vector<HANDLE> activeHandles;

    string getCurrentTimeString();
    string generateMessageOfImageDownoloading(string image, int bites);
    string format(const string& format, ...);

    SOCKET establishConnection(string* host, string* path);

    void writeToConsole(CRITICAL_SECTION* section, string message, ostream* target);
    void writeToLogFile(string stringToWrite);
    void start();
    void runWorkTest();
    void logInfo(const char* messageToWrite);
    void startDownoloadProcess(string* url, int* numlink);

    const int REQUEST_MESSAGE_LENGTH = 70;
    const char* N{};
};


#endif //INC_3_IMAGESERVICE_H
