#include "filecommand.h"

void create(string message, struct filesystem fs) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, e1)) {
       cout << "wrong filename: " << message << endl;
     return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (isFileExist(filename, fs)) {
        cout << filename << " already exist\n";
        return;
    }

    if (!isFreeFileExist(fs)) {
        cout << "filesystem is completed\n";
        return;
    }

    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    while (index < len / 2) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));

        descriptor d;
        memcpy(&d, &b.value, BLOCK_SIZE);

        if (d.isFree == true) {
            d.isFree = false;
            memcpy(&d.filename, filename.c_str(), filename.length());
            memcpy(&b.value, &d, BLOCK_SIZE);
            lseek(fd, index, SEEK_SET);
            write(fd, &b, sizeof(b));
            break;
        }
        index += sizeof(b);
    }
    close(fd);
}
void remove(string message, struct filesystem fs) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, e1)) {
        cout << "wrong filename: " << message << endl;
        return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << filename << " doesn't exist\n";
        return;
    }

    removeFunc(filename, fs);
}
void copy(string message, struct filesystem fs) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, e1)) {
        cout << "wrong filename: " << message << endl;
        return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << filename << " doesn't exist\n";
        return;
    }

    if (!isFreeFileExist(fs)) {
        cout << "filesystem is completed\n";
        return;
    }

    string newFileName = filename;
    while (true) {
        if (newFileName.length() > MAX_FILENAME_LENGTH) {
            cout<<"failed copy\n";
            return;
        }
        newFileName = newFileName.insert(newFileName.length(), "0");
        if (!isFileExist(newFileName, fs) && newFileName.length() <= MAX_FILENAME_LENGTH) {
            break;
        }
    }

    cout<<"new file: "<<newFileName<<endl;
    newFileName = newFileName.insert(0, "\"");
    newFileName = newFileName.insert(newFileName.length(), "\"");
    create(newFileName, fs);
    string message1 = newFileName;
    message1.insert(message1.length(), " \"");
    message1.insert(message1.length(), readFunc(filename, fs));
    message1.insert(message1.length(), "\"");
    write(message1, fs);
}
void move(string message, struct filesystem fs) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);

    string strings[2];
    strings[0] = "";
    strings[1] = "";

    if (regex_search(message.c_str(), m, e1)) {
        strings[0] = m[0];
    }

    if (strings[0] == "") {
        cout << "wrong filename: " << strings[0] << endl;
        return;
    }

    message = regex_replace(message, regex(strings[0]), "");

    if (regex_search(message.c_str(), m, e1)) {
        strings[1] = m[0];
    }

    if (strings[1] == "") {
        cout << "wrong filename: " << strings[1] << endl;
        return;
    }

    string filename = strings[0].substr(1, strings[0].length() - 2);
    string newFileName = strings[1].substr(1, strings[1].length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << filename << " doesn't exist\n";
        return;
    }

    if (isFileExist(newFileName, fs)) {
        cout << filename << " already exist\n";
        return;
    }

    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    while (index < len / 2) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));

        descriptor d;
        memcpy(&d, &b.value, BLOCK_SIZE);

        if (d.isFree == false) {
            if (strcmp(d.filename, filename.c_str()) == 0) {
                memcpy(&d.filename, newFileName.c_str(), newFileName.length());
                memcpy(&b.value, &d, BLOCK_SIZE);
                lseek(fd, index, SEEK_SET);
                write(fd, &b, sizeof(b));
                close(fd);
                break;
            }
        }
        index += sizeof(b);
    }
}
void write(string message, struct filesystem fs) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);

    string strings[2];
    strings[0] = "";
    strings[1] = "";

    if (regex_search(message.c_str(), m, e1)) {
        strings[0] = m[0];
    }

    if (strings[0] == "") {
        cout << "wrong filename: " << strings[0] << endl;
        return;
    }

    message = regex_replace(message, regex(strings[0]), "");

    if (regex_search(message.c_str(), m, regex(DATA))) {
        strings[1] = m[0];
    }

    string filename = strings[0].substr(1, strings[0].length() - 2);
    string data = strings[1].substr(1, strings[1].length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << filename << " doesn't exist\n";
        return;
    }

    writeFunc(filename, data, fs);
}
void read(string message, struct filesystem fs) {
    cmatch m;
    regex e1(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, e1)) {
        cout << "wrong filename: " << message << endl;
        return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << filename << " doesn't exist\n";
        return;
    }

    cout<<readFunc(filename, fs);
    cout<<"\n";
}
void ls(struct filesystem fs) {
    string fileSystemName = fs.fileSystemName;
    int fd = open(fileSystemName.c_str(), O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    int index = 0;
    while (index < len / 2) {
        block b;
        lseek(fd, index, SEEK_SET);
        read(fd, &b, sizeof(b));

        descriptor d;
        memcpy(&d, &b.value, BLOCK_SIZE);

        if (d.isFree == false) {
            cout<<d.filename<<endl;
        }
        index += sizeof(b);
    }
    close(fd);
}