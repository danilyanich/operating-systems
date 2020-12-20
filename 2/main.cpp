include <iostream>
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
            load("http://hdrezka.solutions/i/2014/12/19/o2944592f50b6ef51y13w.jpg");
            load("http://hdrezka.solutions/i/2020/7/6/a5a63b18bfbc6vv43l27a.png");
            load("http://hdrezka.solutions/i/2014/12/19/oa6d180c01826ib58d78n.jpg");
            load("http://hdrezka.solutions/i/2014/12/19/oa6d180c01826ib58d78n.txt");    // NOT AN IMAGE
            load("http://hdrezka.solutions/i/2014/12/19/ffdd1ef7ed796pe87l84k.jpg");
            load("http://hdrezka.solutions/i/2015/5/11/fa3b1af23fde7yq58d96l.jpg");
            load("http://hdrezka.solutions/i/2014/12/19/b075610b581depf12n75h.jpg");
        }
        else {
            load(url);
        }
    }
    outfile.close();
}