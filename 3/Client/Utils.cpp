#pragma once

#include <string>
#include <ctime>
#include <direct.h>

using namespace std;

class Utils {
private:
    Utils() = default;
public:
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
};