#pragma once

#include "CustomFile.cpp"

using namespace std;

class Cluster {
private:

    unsigned long address;

    int size;

    char *data;

    int capacity;

    CustomFile *file;
public:

    Cluster(int size, char *data, int capacity) {
        this->size = size;
        this->data = data;
        this->capacity = capacity;
    }

    Cluster(Cluster *cluster) {
        this->address = cluster->address;
        this->size = cluster->size;
        this->data = cluster->data;
        this->capacity = cluster->capacity;
    }

    unsigned long getAddress() const {
        return address;
    }

    void setAddress(unsigned long address) {
        this->address = address;
    }

    int getSize() const {
        return size;
    }

    void setSize(int size) {
        this->size = size;
    }

    char *getData() const {
        return data;
    }

    void setData(char *data) {
        this->data = data;
    }

    int getCapacity() const {
        return capacity;
    }

    CustomFile *getFile() {
        return file;
    }

    void setFile(CustomFile *file) {
        this->file = file;
    }
};

