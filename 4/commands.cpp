#include "commands.h"

void init(string message) {

    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);
    regex numberReg(NUMBER_REGEXP);

    string strings[2];
    strings[0] = "";
    strings[1] = "";

    if (regex_search(message.c_str(), m, filenameReg)) {
        strings[0] = m[0];
    }

    message = regex_replace(message, regex(strings[0]), "");

    if (regex_search(message.c_str(), m, numberReg)) {
        strings[1] = m[0];
    }

    if (strings[0] == "") {
        cout << "Error! Wrong Filename!" << endl;
        return;
    }
    else if (strings[1] == "") {
        cout << "Error! Wrong Parameter!" << endl;
        return;
    }

    string filename = "bin/";
    filename.append(strings[0].substr(1, strings[0].length() - 2));

    ifstream fin(filename);
    if (!fin.is_open()) {
        int fd = open(filename.c_str(), O_RDWR | O_CREAT);

        int number = ceil((double)stoi(strings[1]) / (double)BLOCK_SIZE) * 2;
        int half = number / 2;
        block* b = new block[number];

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
    }
    else {
        fin.close();
        cout << << "Error! " << filename << " already exist!\n";
        return;
    }
}

string load(string message, struct filesystem fileSystem) {
    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, filenameReg)) {
        cout << "Error! Wrong Filename: " << message << endl;
        return "";
    }

    string filename = "bin/";
    filename.append(message.substr(1, message.length() - 2));

    ifstream fin(filename);

    if (fin.is_open()) {
        fin.close();
        return filename;
    }
    else {
        cout << filename << "Error! Doesn't Exist!\n";
        return "";
    }
}

void create(string message, struct filesystem fileSystem) {
    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, filenameReg)) {
       cout << "Error! Wrong Filename: " << message << endl;
     return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (isFileExist(filename, fs)) {
        cout << filename << "Error! Already Exist!\n";
        return;
    }

    if (!isFreeFileExist(fs)) {
        cout << "Filesystem Is Completed\n";
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
void remove(string message, struct filesystem fileSystem) {
    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, filenameReg)) {
        cout << "Error! Wrong Filename: " << message << endl;
        return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << "Error! " << filename << " Doesn't Exist!\n";
        return;
    }

    removeFunc(filename, fs);
}
void copy(string message, struct filesystem fileSystem) {
    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, filenameReg)) {
        cout << "Error! Wrong Filename: " << message << endl;
        return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << "Error! " << filename << "Doesn't Exist!\n";
        return;
    }

    if (!isFreeFileExist(fs)) {
        cout << "Filesystem Is Completed\n";
        return;
    }

    string newFileName = filename;
    while (true) {
        if (newFileName.length() > MAX_FILENAME_LENGTH) {
            cout << "Failed copy\n";
            return;
        }
        newFileName = newFileName.insert(newFileName.length(), "0");
        if (!isFileExist(newFileName, fs) && newFileName.length() <= MAX_FILENAME_LENGTH) {
            break;
        }
    }

    cout << "New file: "<<newFileName<<endl;
    newFileName = newFileName.insert(0, "\"");
    newFileName = newFileName.insert(newFileName.length(), "\"");
    create(newFileName, fs);
    
    string message1 = newFileName;
    message1.insert(message1.length(), " \"");
    message1.insert(message1.length(), readFunc(filename, fs));
    message1.insert(message1.length(), "\"");
    write(message1, fs);
}

void move(string message, struct filesystem fileSystem) {
    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);

    string strings[2];
    strings[0] = "";
    strings[1] = "";

    if (regex_search(message.c_str(), m, filenameReg)) {
        strings[0] = m[0];
    }

    if (strings[0] == "") {
        cout << "Error! Wrong Filename: " << strings[0] << endl;
        return;
    }

    message = regex_replace(message, regex(strings[0]), "");

    if (regex_search(message.c_str(), m, filenameReg)) {
        strings[1] = m[0];
    }

    if (strings[1] == "") {
        cout << "Error! Wrong Filename: " << strings[1] << endl;
        return;
    }

    string filename = strings[0].substr(1, strings[0].length() - 2);
    string newFileName = strings[1].substr(1, strings[1].length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << "Error! " << filename << " Doesn't exist\n";
        return;
    }

    if (isFileExist(newFileName, fs)) {
        cout << "Error! " << filename << " Already Exist\n";
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
void write(string message, struct filesystem fileSystem) {
    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);

    string strings[2];
    strings[0] = "";
    strings[1] = "";

    if (regex_search(message.c_str(), m, filenameReg)) {
        strings[0] = m[0];
    }

    if (strings[0] == "") {
        cout << "Error! Wrong Filename: " << strings[0] << endl;
        return;
    }

    message = regex_replace(message, regex(strings[0]), "");

    if (regex_search(message.c_str(), m, regex(DATA))) {
        strings[1] = m[0];
    }

    string filename = strings[0].substr(1, strings[0].length() - 2);
    string data = strings[1].substr(1, strings[1].length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << "Error! " << filename << " Doesn't Exist\n";
        return;
    }

    writeFunc(filename, data, fs);
}
void read(string message, struct filesystem fileSystem) {
    cmatch m;
    regex filenameReg(FILE_NAME_REGEXP);

    if (!regex_match(message.c_str(), m, filenameReg)) {
        cout << "Error! Wrong Filename: " << message << endl;
        return;
    }

    string filename = message.substr(1, message.length() - 2);

    if (!isFileExist(filename, fs)) {
        cout << "Error! " << filename << " Doesn't Exist\n";
        return;
    }

    cout<<readFunc(filename, fs);
    cout<<"\n";
}

void ls(struct filesystem fileSystem) {
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