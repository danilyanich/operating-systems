#define UNDEFINED_RESPONSE -1
#define OK_RESPONSE 0
#define ERROR_RESPONSE 1
#define EXIT_RESPONSE 2

#include <cstdlib>
#include <iostream>
#include <string>
#include <regex>
#include <process.h>
#include "ImageDownloader.cpp"
#include "ImageData.cpp"
#include "Utils.cpp"

using namespace std;

ImageDownloader *downloader;

vector<HANDLE> handles;

unsigned __stdcall callDownloader(PVOID param) {
    auto *data = (ImageData*) param;
    downloader->download(data);
    delete data;
    return 0;
}

int download(vector<string>* args) {
    string url = args->at(0);
    string path = args->size() == 2? args->at(1) : Utils::getDefaultPath(url);
    auto *data = new ImageData(url, path);
    unsigned threadId;
    auto handle = (HANDLE) _beginthreadex(nullptr, 0, &callDownloader, (PVOID) data, 0, &threadId);
    handles.push_back(handle);
    return OK_RESPONSE;
}

int download(const string& url) {
    auto *data = new ImageData(url, Utils::getDefaultPath(url));
    unsigned threadId;
    auto handle = (HANDLE) _beginthreadex(nullptr, 0, &callDownloader, (PVOID) data, 0, &threadId);
    handles.push_back(handle);
    return OK_RESPONSE;
}

void test() {
    vector<string> urls;
    urls.push_back("https://st4.depositphotos.com/2435561/23030/i/1600/depositphotos_230304352-stock-photo-red-sign-night-chinatown-milan.jpg");
    urls.push_back("https://st3.depositphotos.com/15883498/36797/v/1600/depositphotos_367970838-stock-illustration-woman-lifestyle-bathroom-luxury-vegetation.jpg");
    urls.push_back("https://st4.depositphotos.com/10484378/25244/i/1600/depositphotos_252445038-stock-photo-awesome-sunset-on-beautiful-dolomite.jpg");
    urls.push_back("https://st4.depositphotos.com/17822240/38980/v/1600/depositphotos_389801634-stock-illustration-seamless-pattern-with-hand-drawn.jpg");
    for (auto & url : urls) {
        download(url);
    }
}

int service(const string& expression) {
    vector<string> tokens;
    Utils::split(expression, ' ', &tokens);
    int response = UNDEFINED_RESPONSE;
    if (tokens.size() == 1 && tokens[0] == "test") {
        test();
        response = OK_RESPONSE;
    }
    if (tokens.size() == 1 && tokens[0] == "exit") {
        response = EXIT_RESPONSE;
    }
    if ((tokens.size() == 2 || tokens.size() == 1) && response == UNDEFINED_RESPONSE) {
        response = download(&tokens);
    }
    if (response == UNDEFINED_RESPONSE || tokens.empty()) {
        response = ERROR_RESPONSE;
    }
    return response;
}

void close() {
    WaitForMultipleObjects(handles.size(), handles.data(), true, INFINITE);
    for (auto & handle : handles) {
        CloseHandle(handle);
    }
}

int main() {
    cout << "Started" << endl;
    downloader = new ImageDownloader();
    int response;
    while (true) {
        string expression;
        getline(cin, expression);
        response = service(expression);
        if (response == EXIT_RESPONSE) {
            close();
            cout << "Exited" << endl;
            response = OK_RESPONSE;
            break;
        }
        if (response == ERROR_RESPONSE) {
            cout << "Incorrect command" << endl;
        }
    }
    delete downloader;
    exit(response);
}