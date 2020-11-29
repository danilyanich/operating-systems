#pragma once

#include <string>
#include <process.h>
#include <deque>
#include "Utils.cpp"

using namespace std;

class Logger {

public:
    explicit Logger(const string &pathToLogFile) {
        fileStream = ofstream(pathToLogFile);
        isWork = true;
        logs = new deque<string>();
        InitializeCriticalSection(&queueSection);
        handle = (HANDLE) _beginthreadex(nullptr, 0, &Logger::listenLogs, (PVOID) this, 0, &threadId);
    }

    void info(const string &message) {
        string formattedMessage = "INFO: " + Utils::getTime() + " : " + message;
        saveLog(formattedMessage);
    }

    void error(const string &message) {
        string formattedMessage = "WARN: " + Utils::getTime() + " : " + message;
        saveLog(formattedMessage);
    }

    void close() {
        isWork = false;
        WaitForSingleObject(handle, INFINITE);
        CloseHandle(handle);
        fileStream.close();
        DeleteCriticalSection(&queueSection);
    }

private:

    ofstream fileStream;

    CRITICAL_SECTION queueSection;

    deque<string> *logs;

    unsigned threadId{};

    HANDLE handle;

    bool isWork;

    void saveLog(const string &message) {
        EnterCriticalSection(&queueSection);
        logs->push_back(message);
        LeaveCriticalSection(&queueSection);
    }

    bool isActive() {
        return isWork || !logs->empty();
    }

    static unsigned __stdcall listenLogs(PVOID param) {
        auto *logger = (Logger*) param;
        deque<string> *logs = logger->logs;
        while (true) {
            if (!logger->isActive()) {
                break;
            }
            while (!logs->empty()) {
                logger->log();
            }
        }
        return 0;
    }

    void log() {
        fileStream << logs->front() << endl;
        logs->pop_front();
    }
};