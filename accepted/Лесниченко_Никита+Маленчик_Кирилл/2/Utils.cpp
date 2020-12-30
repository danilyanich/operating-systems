#pragma once

#define OK_CODE 200
#define REDIRECT_CODE 300
#define BAD_REQUEST_CODE 400
#define SERVER_ERROR_CODE 500

#include <string>
#include <dirent.h>
#include <ctime>

using namespace std;

class Utils {

public:
    static string getDefaultPath(const string &url) {
        return getCurrentPath() + "\\" + getNameOfImageFromUrl(url) + getExtensionOfImageFromUrl(url);
    }

    static string getCurrentPath() {
        return _getcwd(nullptr, 0);
    }

    static void split(const string &source, char delimiter, vector<string>* result) {
        stringstream stream(source);
        string substring;
        while (getline(stream, substring, delimiter)) {
            result->push_back(substring);
        }
    }

    static int normalizeStatusCode(int statusCode) {
        if (statusCode >= OK_CODE && statusCode < REDIRECT_CODE) {
            return OK_CODE;
        }
        if (statusCode >= REDIRECT_CODE && statusCode < BAD_REQUEST_CODE) {
            return REDIRECT_CODE;
        }
        if (statusCode >= BAD_REQUEST_CODE && statusCode < SERVER_ERROR_CODE) {
            return BAD_REQUEST_CODE;
        } else {
            return SERVER_ERROR_CODE;
        }
    }

    static bool isValidUrl(const string& url) {
        regex pattern("^(https?://)(.+)(/(.+)[.].+)$");
        return regex_match(url, pattern);
    }

    static string getTime() {
        time_t nowTime = time(nullptr);
        char buffer[80];
        tm tm = *localtime(&nowTime);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d.%X", &tm);
        return buffer;
    }

private:
    static string getExtensionOfImageFromUrl(const string &url) {
        regex extensionPattern("^.*://([^/?:]+)(/?.*)([.].+)+$");
        return regex_replace(url, extensionPattern, "$3");
    }

    static string getNameOfImageFromUrl(const string &url) {
        regex namePattern("^.*://([^/?:]+)(/?.*)/+(.+)+([.].+)+$");
        return regex_replace(url, namePattern, "$3");
    }
};