#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "Utils.cpp"

using namespace std;

class Logger {
public:
    explicit Logger(const string &pathToLogFile, bool useConsole) {
        fileStream = ofstream(pathToLogFile);
        this->useConsole = useConsole;
        InitializeCriticalSection(&section);
    }

    ~Logger() {
        DeleteCriticalSection(&section);
        fileStream.close();
    }

    void info(const string &message) {
        string formattedMessage = "INFO  " + Utils::getTime() + " : " + message;
        log(formattedMessage);
    }

    void warn(const string &message) {
        string formattedMessage = "WARN  " + Utils::getTime() + " : " + message;
        log(formattedMessage);
    }

    void error(const string &message) {
        string formattedMessage = "ERROR " + Utils::getTime() + " : " + message;
        log(formattedMessage);
    }
private:

    bool useConsole;

    ofstream fileStream;

    CRITICAL_SECTION section{};

    void log(const string &message) {
        EnterCriticalSection(&section);
        fileStream << message << endl;
        if (useConsole) {
            cout << message << endl;
        }
        LeaveCriticalSection(&section);
    }
};