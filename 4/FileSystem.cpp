#include <iostream>
#include <vector>
#include <Windows.h>
#include "Directory.cpp"
#include "Cluster.cpp"

class FileSystem {
private:
    Directory* root;
    Directory* current;
    File* bufferFile = NULL;
    Directory* bufferDirectory = NULL;
    Directory* tempDirectory = NULL;
    std::vector<Cluster> clusters;
    int oneClusterSize;

    bool isCopy = false;

    int findFreeCluster() {
        for (int i = 0; i < clusters.size(); i++) {
            if (clusters[i].isFree()) {
                return i;
            }
        }
        return -1;
    }

    void deleteFile(File* file){
        freeClusters(file->getClusterNumbers());  
        delete file;
    }

    void deleteDirectory(Directory* directory) {
        std::vector<Directory*> directories = directory->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            directory->removeDirectory((*directories[i]).getDirectoryName());
            deleteDirectory(directories[i]);
        }
        std::vector<File*> files = directory->getFiles();
        for (int i = 0; i < files.size(); i++) {
            directory->removeFile((*files[i]).getFileName());
            deleteFile(files[i]);
        }
        freeClusters(directory->getClusterNumbers());
        delete directory;
    }

    void freeClusters(std::vector<int> clusterNumbers) {
        for (int i = 0; i < clusterNumbers.size(); i++) {
            clusters[clusterNumbers[i]].freeCluster();
            clusters[clusterNumbers[i]].freeClusterInfo();
        }
    }

    bool findFileName(std::string name) {
        std::vector<File*> files = current->getFiles();
        for (int i = 0; i < files.size(); i++) {
            if ((*files[i]).getFileName() == name) {
                return true;
            }
        }
        return false;
    }

    bool findDirectoryName(std::string name) {
        std::vector<Directory*> directories = current->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            if ((*directories[i]).getDirectoryName() == name) {
                return true;
            }
        }
        return false;
    }

    void write(File* file, std::string info) {
        std::vector<int> clusterNubmers = file->getClusterNumbers();

        int number = info.length() % oneClusterSize;
        if (number != 0) {
            number = info.length() + (oneClusterSize - number);
        }
        else number = info.length();
        number /= oneClusterSize;

        while (clusterNubmers.size() < number) {
            int clusterNumber = findFreeCluster();
            if (clusterNumber == -1) return;
            clusters[clusterNumber].takeCluster();
            clusterNubmers.push_back(clusterNumber);
        }
        while (clusterNubmers.size() > number) {
            clusters[clusterNubmers.back()].freeCluster();
            clusterNubmers.pop_back();
        }

        int start = 0;
        for (int i = 0; i < clusterNubmers.size(); i++) {
            clusters[clusterNubmers[i]].freeClusterInfo();
            clusters[clusterNubmers[i]].setClusterInfo(info.substr(start, oneClusterSize));
            start += oneClusterSize;
        }

        file->setClusterNumbers(clusterNubmers);
    }

    std::string read(File* file) {
        std::vector<int> clusterNubmers = file->getClusterNumbers();
        std::string str = "";
        for (int i = 0; i < clusterNubmers.size(); i++) {
            str += clusters[clusterNubmers[i]].getClusterInfo();
        }
        return str;
    }

    void copyDirectory(Directory* copy, Directory* directory) {
        std::vector<Directory*> directories = directory->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            int clusterNumber = findFreeCluster();
            if (clusterNumber != -1) {
                clusters[clusterNumber].takeCluster();
                Directory* temp = new Directory(clusterNumber, copy, (*directories[i]).getDirectoryName());
                copyDirectory(temp, directories[i]);
                copy->addDirectory(temp);
            }
        }
        std::vector<File*> files = directory->getFiles();
        for (int i = 0; i < files.size(); i++) {
            int clusterNumber = findFreeCluster();
            if (clusterNumber != -1) {
                clusters[clusterNumber].takeCluster();
                File* file = new File(clusterNumber, (*files[i]).getFileName());
                write(file, read(files[i]));
                copy->addFile(file);
            }
        }
    }
public: 
    FileSystem(int memory, int clusterSize) {
        oneClusterSize = clusterSize;
        for (int i = 0; i < memory / clusterSize; i++) {
            Cluster cluster(i, clusterSize);
            clusters.push_back(cluster);
        }
        int clusterNumber = findFreeCluster();
        clusters[clusterNumber].takeCluster();
        root = new Directory(clusterNumber, NULL, "root");
        current = root;
        root->updateClusterNumbers();
    }
    
    void createFile(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!findFileName(action[1])) {
            int clusterNumber = findFreeCluster();
            if (clusterNumber != -1) {
                clusters[clusterNumber].takeCluster();
                File* file = new File(clusterNumber, action[1]);
                current->addFile(file);
                root->updateClusterNumbers();
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "file created\n";
            }
            else {
                SetConsoleTextAttribute(handle, FOREGROUND_RED);
                std::cout << "no more memory\n";
            }
        }
        else {
            SetConsoleTextAttribute(handle, FOREGROUND_RED);
            std::cout << "file with this name exist\n";
        }
    }

    void createDirectory(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (!findDirectoryName(action[1])) {
            int clusterNumber = findFreeCluster();
            if (clusterNumber != -1) {
                clusters[clusterNumber].takeCluster();
                Directory* directory = new Directory(clusterNumber, current, action[1]);
                current->addDirectory(directory);
                root->updateClusterNumbers();
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "directory created\n";
            }
            else {
                SetConsoleTextAttribute(handle, FOREGROUND_RED);
                std::cout << "no more memory\n";
            }
        }
        else {
            SetConsoleTextAttribute(handle, FOREGROUND_RED);
            std::cout << "directory with this name exist\n";
        }
    }

    void showDirectory() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
        std::cout << "Files:\n";
        std::vector<File*> files = current->getFiles();
        for (int i = 0; i < files.size(); i++) {
            std::cout << (*files[i]).getFileName() << std::endl;
        }
        std::cout << "Directories:\n";
        std::vector<Directory*> directories = current->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            std::cout << (*directories[i]).getDirectoryName() << std::endl;
        }
    }

    void changeDirectory(std::vector<std::string> action) {
        if (action[1] == "..") {
            if (current->getParent() != NULL) {
                current = current->getParent();
            }
        }
        else {
            std::vector<Directory*> directories = current->getDirectories();
            for (int i = 0; i < directories.size(); i++) {
                if ((*directories[i]).getDirectoryName() == action[1]) {
                    current = directories[i];
                }
            }
        }
    }

    void deleteFile(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<File*> files = current->getFiles();
        for (int i = 0; i < files.size(); i++) {
            if ((*files[i]).getFileName() == action[1]) {
                current->removeFile(action[1]);
                deleteFile(files[i]);
                root->updateClusterNumbers();
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "file deleted\n";
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "file not found\n";
    }

    void deleteDirectory(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<Directory*> directories = current->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            if ((*directories[i]).getDirectoryName() == action[1]) {
                current->removeDirectory(action[1]);
                deleteDirectory(directories[i]);
                root->updateClusterNumbers();
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "directory deleted\n";
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "directory not found\n";
    }

    void cutFile(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<File*> files = current->getFiles();
        for (int i = 0; i < files.size(); i++) {
            if ((*files[i]).getFileName() == action[1]) {
                tempDirectory = current;
                bufferDirectory = NULL;
                bufferFile = files[i];
                isCopy = false;
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "file in buffer\n";
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "file not found\n";
    }

    void copyFile(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<File*> files = current->getFiles();
        for (int i = 0; i < files.size(); i++) {
            if ((*files[i]).getFileName() == action[1]) {
                tempDirectory = current;
                bufferDirectory = NULL;
                bufferFile = files[i];
                isCopy = true;
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "file in buffer\n";
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "file not found\n";
    }

    void pasteFile() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (bufferFile != NULL) {
            if (!isCopy) {
                tempDirectory->removeFile(bufferFile->getFileName());
                while (findFileName(bufferFile->getFileName())) {
                    bufferFile->setFileName(bufferFile->getFileName() + "Copy");
                }
                current->addFile(bufferFile);
            }
            else {
                int clusterNumber = findFreeCluster();
                if (clusterNumber != -1) {
                    clusters[clusterNumber].takeCluster();
                    File* file = new File(clusterNumber, bufferFile->getFileName());
                    write(file, read(bufferFile));
                    while (findFileName(file->getFileName())) {
                        file->setFileName(file->getFileName() + "Copy");
                    }
                    current->addFile(file);
                }
            }
            root->updateClusterNumbers();
            bufferFile = NULL;
            tempDirectory = NULL;
            isCopy = false;
            SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
            std::cout << "file inserted\n";
        }
        else {
            SetConsoleTextAttribute(handle, FOREGROUND_RED);
            std::cout << "no file in buffer\n";
        }
    }

    void cutDirectory(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<Directory*> directories = current->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            if ((*directories[i]).getDirectoryName() == action[1]) {
                tempDirectory = current;
                bufferDirectory = directories[i];
                bufferFile = NULL;
                isCopy = false;
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "directory in buffer\n";
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "directory not found\n";
    }

    void copyDirectory(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<Directory*> directories = current->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            if ((*directories[i]).getDirectoryName() == action[1]) {
                tempDirectory = current;
                bufferDirectory = directories[i];
                bufferFile = NULL;
                isCopy = true;
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "directory in buffer\n";
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "directory not found\n";
    }

    void pasteDirectory() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        if (bufferDirectory != NULL) {
            if (!isCopy) {
                tempDirectory->removeDirectory(bufferDirectory->getDirectoryName());
                while (findDirectoryName(bufferDirectory->getDirectoryName())) {
                    bufferDirectory->setDirectoryName(bufferDirectory->getDirectoryName() + "Copy");
                }
                current->addDirectory(bufferDirectory);
            }
            else {
                int freeClusterts = 0;
                for (int i = 0; i < clusters.size(); i++) {
                    if (clusters[i].isFree()) {
                        freeClusterts++;
                    }
                }
                if (bufferDirectory->getClusterNumbers().size() <= freeClusterts) {
                    int clusterNumber = findFreeCluster();
                    if (clusterNumber != -1) {
                        clusters[clusterNumber].takeCluster();
                        Directory* directory = new Directory(clusterNumber, current, bufferDirectory->getDirectoryName());
                        while (findDirectoryName(directory->getDirectoryName())) {
                            directory->setDirectoryName(directory->getDirectoryName() + "Copy");
                        }
                        copyDirectory(directory, bufferDirectory);
                        current->addDirectory(directory);
                        root->updateClusterNumbers();
                    }
                }
                else return;
            }
            bufferDirectory = NULL;
            tempDirectory = NULL;
            isCopy = false;
            SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
            std::cout << "directory inserted\n";
        }
        else {
            SetConsoleTextAttribute(handle, FOREGROUND_RED);
            std::cout << "no directory in buffer\n";
        }
    }

    void writeFile(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<File*> files = current->getFiles();
        for (int i = 0; i < files.size(); i++) {
            if ((*files[i]).getFileName() == action[1]) {
                write(files[i], action[2]);
                root->updateClusterNumbers();
                SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
                std::cout << "file changed\n";
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "file not found\n";
    }

    void readFile(std::vector<std::string> action) {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        std::vector<File*> files = current->getFiles();
        for (int i = 0; i < files.size(); i++) {
            if ((*files[i]).getFileName() == action[1]) {
                SetConsoleTextAttribute(handle, FOREGROUND_GREEN);
                std::cout << read(files[i]) << std::endl;
                return;
            }
        }
        SetConsoleTextAttribute(handle, FOREGROUND_RED);
        std::cout << "file not found\n";
    }

    std::string getPath() {
        std::vector<std::string> directories;
        Directory* temp = current;
        do {
            directories.push_back(temp->getDirectoryName());
            temp = temp->getParent();
        } while (temp != NULL);
        std::string path = "";
        while (!directories.empty()) {
            path += directories.back() + "\\";
            directories.pop_back();
        }
        return path;
    }

    void dummyDump() {
        for (int i = 0; i < clusters.size(); i++) {
            std::cout << clusters[i].getClusterIndex() << " cluste info: " << clusters[i].getClusterInfo() << std::endl;
        }
    }
};