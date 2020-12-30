#include <iostream>
#include <vector>

class File {
private:
    std::vector<int> clusterNumbers;
    std::string fileName;
public:
    File(int clusterNumber, std::string name) {
        clusterNumbers.push_back(clusterNumber);
        fileName = name;
    }

    void setClusterNumbers(std::vector<int> numbers) {
        clusterNumbers = numbers;
    }

    std::vector<int> getClusterNumbers() {
        return clusterNumbers;
    }
    
    std::string getFileName() {
        return fileName;
    }
    
    void setFileName(std::string newName) {
        fileName = newName;
    }
};