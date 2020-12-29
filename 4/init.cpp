#include "init.h"
#include <math.h>

void init(string message) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);
    regex e2(NUMBER_REGEXP);

    string strings[2];
    strings[0] = "";
    strings[1] = "";
    if (regex_search(message.c_str(), m, e1)) {
        strings[0] = m[0];
    }

    message = regex_replace(message, regex(strings[0]), "");

    if (regex_search(message.c_str(), m, e2)) {
        strings[1] = m[0];
    }
    if (strings[0] == "") {
        cout << "wrong filename" << endl;
        return;
    }
    if (strings[1] == "") {
        cout << "wrong parameter" << endl;
        return;
    }

    string filename = "bin/";
    filename.append(strings[0].substr(1, strings[0].length() - 2));
    ifstream fin(filename);

    if (!fin.is_open()) {
        int fd = open(filename.c_str(), O_RDWR | O_CREAT);

        int number = ceil((double) stoi(strings[1]) / (double) BLOCK_SIZE) * 2;
        int half = number / 2;
        block *b = new block[number];

        for (int index = 0; index < half; index++) {
            descriptor d;
            b[index].freeSpace = 0;
            memcpy(&b[index].value, &d, BLOCK_SIZE);
        }

        for (int index = 0; index < number; index++) {
            b[index].blockIndex = index;
            write(fd, &b[index], sizeof(b[index]));
        }
        close(fd);
    } else {
        fin.close();
        cout << filename << " already exist\n";
        return;
    }
}