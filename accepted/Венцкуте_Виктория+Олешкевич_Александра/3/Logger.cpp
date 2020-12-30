#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include "Utils.h"
#include <synchapi.h>
#include <synchapi.h>

using namespace std;

class Logger {
public:
    Logger(const string &pathToLogFile, bool useConsole) {
        fileStream = ofstream(pathToLogFile);
        this->useConsole = useConsole;
        InitializeCriticalSection(&section);
    }

    ~Logger() {
        DeleteCriticalSection(&section);
        fileStream.close();
    }

    void info(const string &message) {
        string formattedMessage = getTime() + " [info]" + " - " + message;
        SetConsoleTextAttribute(hConsole, 15);  // white log text
        log(formattedMessage);
    }

    void warn(const string &message) {
        string formattedMessage = getTime() + " [warn]" + " - " + message;
        SetConsoleTextAttribute(hConsole, 14);  // yellow warning
        log(formattedMessage);
    }

    void error(const string &message) {
        string formattedMessage = getTime() + " [error]" + " - " + message;
        SetConsoleTextAttribute(hConsole, 12);  // red error
        log(formattedMessage);
    }
private:
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
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