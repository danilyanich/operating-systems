#include "load.h"

string load(string message, struct filesystem fs) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, e1)) {
        cout << "wrong filename: " << message << endl;
        return "";
    }

    string filename = "bin/";
    filename.append(message.substr(1, message.length() - 2));

    ifstream fin(filename);
    if (fin.is_open()) {
        fin.close();
        return filename;
    } else {
        cout << filename << " doesn't exist\n";
        return "";
    }
}