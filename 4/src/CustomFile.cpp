#pragma once

#include <vector>

#define DEFAULT_ADDRESSES_SIZE 0
#define DEFAULT_SIZE 0
#define DEFAULT_TOTAL_ADDRESSES 0

using namespace std;

class CustomFile {
private:
    string name;

    unsigned long size = DEFAULT_SIZE;

    unsigned long totalAddresses = DEFAULT_TOTAL_ADDRESSES;

    vector<unsigned long> *addresses = new vector<unsigned long>(DEFAULT_ADDRESSES_SIZE);

public:
    CustomFile(const string &name) {
        this->name = name;
    }

    const string &getName() {
        return name;
    }

    void setName(const string &name) {
        this->name = name;
    }

    unsigned long getSize() {
        return size;
    }

    void setSize(unsigned long size) {
        this->size = size;
    }

    void setTotalAddresses(unsigned long totalAddresses) {
        this->totalAddresses = totalAddresses;
    }

    vector<unsigned long> *getAddresses() {
        return addresses;
    }
};

