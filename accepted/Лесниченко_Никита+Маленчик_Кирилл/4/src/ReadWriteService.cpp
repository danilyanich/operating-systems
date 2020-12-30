#pragma once

#include "InterfaceCluster.cpp"
#include <vector>
#include <cstring>
#include <iostream>

#define EMPTY_CLUSTER_SIZE 0

class ReadWriteService {
private:
    InterfaceCluster *interfaceCluster;
public:

    void setInterfaceCluster(InterfaceCluster *clusterDao) {
        this->interfaceCluster = clusterDao;
    }

    vector<Cluster *> *readClusters(CustomFile *file) {
        auto *clusters = new vector<Cluster *>();
        for (auto &address : *file->getAddresses()) {
            clusters->push_back(new Cluster(interfaceCluster->findCluster(address)));
        }
        return clusters;
    }

    void printClusters(vector<Cluster *> *clusters) {
        for (auto &cluster : *clusters) {
            for (int index = 0; index < cluster->getSize(); index++) {
                cout << cluster->getData()[index];
            }
        }
    }

    void writeClusters(CustomFile *file, char *data, int size, vector<Cluster *> *existedClusters) {
        int clusterCapacity = interfaceCluster->getCapacityOfCluster();
        int offset = fillExistedClusters(data, size, existedClusters);
        file->setSize(file->getSize() + offset);
        size -= offset;
        int fullClusters = size / clusterCapacity;
        char *buffer = data + offset;
        auto *newClusters = new vector<Cluster *>();
        for (int clusterIndex = 0; clusterIndex < fullClusters; clusterIndex++) {
            char *clusterData = new char[clusterCapacity];
            memcpy(clusterData, buffer, clusterCapacity);
            auto *cluster = new Cluster(clusterCapacity, clusterData, clusterCapacity);
            newClusters->push_back(cluster);
            buffer += clusterCapacity;
        }
        fillPartialCluster(size, fullClusters, clusterCapacity, buffer, newClusters);
        fillFile(file, newClusters);
    }

    void fillFile(CustomFile *file, vector<Cluster *> *clusters) {
        unsigned long fileSize = file->getSize();
        for (auto &cluster : *clusters) {
            file->getAddresses()->push_back(interfaceCluster->saveCluster(cluster));
            cluster->setFile(file);
            fileSize += cluster->getSize();
        }
        file->setSize(fileSize);
        file->setTotalAddresses(clusters->size());
    }

private:
    int fillExistedClusters(char *data, int size, vector<Cluster *> *existedClusters) {
        if (!existedClusters->empty()) {
            Cluster *cluster = existedClusters->back();
            int offset = addToCluster(cluster, data, size);
            cluster->setSize(cluster->getSize() + offset);
            if (offset != EMPTY_CLUSTER_SIZE) {
                interfaceCluster->updateCluster(cluster->getAddress(), cluster);
            }
            return offset;
        } else {
            return EMPTY_CLUSTER_SIZE;
        }
    }

    int addToCluster(Cluster *cluster, char *data, int size) {
        int sizeOfCluster = cluster->getSize();
        int capacityOfCluster = cluster->getCapacity();
        int indexOfData = 0;
        for (int index = sizeOfCluster; index < capacityOfCluster && size > 0; index++) {
            cluster->getData()[index] = data[indexOfData++];
            size--;
        }
        return indexOfData;
    }

    void fillPartialCluster(int size, int fullClusters, int clusterCapacity,
                            char *buffer, vector<Cluster *> *clusters) {
        int sizeOfPartialCluster = size - fullClusters * clusterCapacity;
        if (sizeOfPartialCluster != EMPTY_CLUSTER_SIZE) {
            char *clusterData = new char[sizeOfPartialCluster];
            memcpy(clusterData, buffer, sizeOfPartialCluster);
            auto *cluster = new Cluster(sizeOfPartialCluster, clusterData, clusterCapacity);
            clusters->push_back(cluster);
        }
    }
};
