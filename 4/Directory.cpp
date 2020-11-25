#include <iostream>
#include <vector>
#include "File.cpp"

class Directory {
private:
    std::vector<int> clusterNumbers;
    std::string directoryName;
    Directory* parent;
    std::vector<File*> files;
    std::vector<Directory*> directories;

    std::vector<int> calculateClusterNumbers(Directory* directory, std::vector<int> numbers) {
        std::vector<Directory*> directories = directory->getDirectories();
        for (int i = 0; i < directories.size(); i++) {
            (*directories[i]).updateClusterNumbers();
            std::vector<int> temp = (*directories[i]).getClusterNumbers();
            for (int j = 0; j < temp.size(); j++) {
                numbers.push_back(temp[j]);
            }
        }
        std::vector<File*> files = directory->getFiles();
        for (int i = 0; i < files.size(); i++) {
            std::vector<int> temp = (*files[i]).getClusterNumbers();
            for (int j = 0; j < temp.size(); j++) {
                numbers.push_back(temp[j]);
            }
        }
        return numbers;
    }
public:
    Directory(int clusterNumber, Directory* dir, std::string name) {
        clusterNumbers.push_back(clusterNumber);
        parent = dir;
        directoryName = name;
    }

    std::vector<int> getClusterNumbers() {
        return clusterNumbers;
    }

    void updateClusterNumbers() {
        clusterNumbers.erase(clusterNumbers.begin() + 1, clusterNumbers.end());
        clusterNumbers = calculateClusterNumbers(this, clusterNumbers);
    }

    std::string getDirectoryName() {
        return directoryName;
    }

    std::vector<File*> getFiles() {
        return files;
    }

    std::vector<Directory*> getDirectories() {
        return directories;
    }

    void setDirectoryName(std::string newDirectoryName) {
        directoryName = newDirectoryName;
    }

    void addFile(File* file) {
        files.push_back(file);
    }

    void removeFile(std::string fileName) {
        for (int i = 0; i < files.size(); i++)
        {
            if ((*files[i]).getFileName() == fileName) {
                files.erase(files.begin() + i);
            }
        }
    }

    void addDirectory(Directory* directory) {
        directories.push_back(directory);
    }

    void removeDirectory(std::string dirName) {
        for (int i = 0; i < directories.size(); i++)
        {
            if ((*directories[i]).getDirectoryName() == dirName) {
                directories.erase(directories.begin() + i);
            }
        }
    }

    Directory* getParent() {
        return parent;
    }
};