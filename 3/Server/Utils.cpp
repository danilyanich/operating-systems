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

class Utils {
private:
    Utils() = default;

public:
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

    static void dump(vector<string> &buffer) {
        CreateDirectory("tmp", nullptr);
        ofstream fileStream = ofstream("tmp\\" + createTemporaryFileName());
        for (auto &message : buffer) {
            fileStream << message << endl;
        }
        fileStream.close();
        buffer.clear();
    }

private:
    static string createTemporaryFileName() {
        char tmpname[L_tmpnam];
        char* filename = tmpnam(tmpname);
        fprintf(stdout, "Temporary file name is: %s", filename);
        return filename;
    }
};