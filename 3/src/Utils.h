#pragma once

#include <string>
#include <ctime>
#include <direct.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <cstdio>

using namespace std;

static string generateLine(int length) {
    string sourceSet = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    string line;
    srand(time(nullptr));
    for (int index = 0; index < length; index++) {
        line += sourceSet[rand() % sourceSet.size()];
    }
    return line;
}

static string getCurrentPath() {
    return _getcwd(nullptr, 0);
}

static string getTime() {
    time_t nowTime = time(nullptr);
    char buffer[80];
    tm tm = *localtime(&nowTime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &tm);
    return buffer;
}

static string getCurrentThreadHandle() {
    HANDLE handle;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &handle,
        DUPLICATE_SAME_ACCESS, FALSE, DUPLICATE_SAME_ACCESS);
    stringstream threadHandle;
    threadHandle << handle;
    return threadHandle.str();
}