#include <iostream>
#include <thread>
#include <windows.h>
#include <shellapi.h>
#include <mutex>
#include <urlmon.h>
#include <fstream>
#pragma comment(lib, "urlmon.lib")

using namespace std;

mutex locker;
ofstream outfile;

const string images[8] = { "jpg", "jpeg", "png", "bmp", "webp", "ico", "tiff", "svg" };

void logger(string log) {
    locker.lock();
    outfile << log << endl;
    locker.unlock();
}

void get_image(string URL) {
    const size_t slash = URL.find_last_of("\\/");
    const string filename = URL.substr(slash + 1);
    logger("processing file " + filename);

    const size_t dot = filename.find_last_of(".");
    const string extension = filename.substr(dot + 1);
    bool isImage = false;

    for (int i = 0; i < 8; i++) {
        if (images[i] == extension) {
            logger("entered URL(" + URL + ") contains an image with extension " + extension);
            isImage = true;
            break;
        }
    }

    if (!isImage) {
        logger("entered URL(" + URL + ") is not an image!");
        return;
    }
    
    if (!URLDownloadToFile(NULL, URL.c_str(), filename.c_str(), BINDF_GETNEWESTVERSION, NULL)) {
        logger("successfully loaded " + filename);

        logger("opening loaded image " + filename + " in a viewer");
        locker.lock();
        ShellExecute(0, "open", filename.c_str(), NULL, NULL, SW_SHOWDEFAULT);
        locker.unlock();
    }
    else logger("DOWNLOAD FAILED FOR URL " + URL);
}

void load(string URL) {
    thread (get_image, URL).detach();
}

int main() {
    outfile.open("loader.log", ios::out | ios::trunc);
    string url;
    while (true) {
        cout << "Enter image URL ('test' for tests): ";
        cin >> url;
        if (url == "test") {
            load("https://homepages.cae.wisc.edu/~ece533/images/fruits.png");
            load("https://homepages.cae.wisc.edu/~ece533/images/airplane.png");
            load("https://homepages.cae.wisc.edu/~ece533/images/boat.png");
            load("https://homepages.cae.wisc.edu/~ece533/images/p64int.txt");    // NOT AN IMAGE
            load("https://homepages.cae.wisc.edu/~ece533/images/sails.png");
            load("https://homepages.cae.wisc.edu/~ece533/images/mountain.png");
            load("https://homepages.cae.wisc.edu/~ece533/images/monarch.png");
        }
        else {
            load(url);
        }
    }
    outfile.close();
}