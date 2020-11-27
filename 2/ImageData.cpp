
#pragma once

#include<string>

using namespace std;

class ImageData {
private:
    string url;
    string path;
public:

    ImageData(const string &urlArg, const string &pathArg) {
        url = urlArg;
        path = pathArg;
    }

    void setUrl(const string &arg) {
        url = arg;
    }

    string &getUrl() {
        return url;
    }

    void setPath(const string &arg) {
        path = arg;
    }

    string &getPath() {
        return path;
    }
};