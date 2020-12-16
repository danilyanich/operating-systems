#pragma once

#include "ClusterNotFoundException.cpp"
#include "OutOfMemoryException.cpp"
#include "Cluster.cpp"

#define MINIMAL_ADDRESS 0
#define DEFAULT_CAPACITY 16
#define DEFAULT_TOTAL_MEMORY 1024

class InterfaceCluster {
private:
    vector<Cluster *> *clusters;

    int capacityOfCluster;

    int totalClusters;

    int capturedClusters;
public:

    InterfaceCluster() {
        InterfaceCluster(DEFAULT_CAPACITY, DEFAULT_TOTAL_MEMORY);
    }

    InterfaceCluster(int capacityOfCluster, int totalMemory) {
        this->capacityOfCluster = capacityOfCluster;
        totalClusters = totalMemory / capacityOfCluster;
        capturedClusters = 0;
        clusters = new vector<Cluster *>(totalClusters);
    }

    int getCapacityOfCluster() {
        return capacityOfCluster;
    }

    vector<Cluster *> *findAll() {
        return new vector<Cluster *>(*clusters);
    }

    Cluster *findCluster(unsigned long address) {
        if (address >= MINIMAL_ADDRESS && address < totalClusters) {
            return clusters->at(address);
        } else {
            throw ClusterNotFoundException();
        }
    }

    unsigned long saveCluster(Cluster *cluster) {
        if (capturedClusters < totalClusters) {
            for (int addressOfCluster = 0; addressOfCluster < totalClusters; addressOfCluster++) {
                if (clusters->at(addressOfCluster) == nullptr) {
                    clusters->at(addressOfCluster) = cluster;
                    cluster->setAddress(addressOfCluster);
                    increaseCapturedClusters();
                    return addressOfCluster;
                }
            }
        }
        throw OutOfMemoryException();
    }

    void updateCluster(unsigned long address, Cluster *cluster) {
        Cluster *existedCluster = findCluster(address);
        existedCluster->setSize(cluster->getSize());
        existedCluster->setData(cluster->getData());
    }

    void deleteCluster(unsigned long address) {
        if (address >= MINIMAL_ADDRESS && address < totalClusters) {
            delete clusters->at(address);
            clusters->at(address) = nullptr;
            decreaseCapturedClusters();
        } else {
            throw ClusterNotFoundException();
        }
    }

    void deleteAllClusters(vector<unsigned long> *addresses) {
        for (auto &address : *addresses) {
            deleteCluster(address);
        }
    }

private:
    void increaseCapturedClusters() {
        capturedClusters++;
    }

    void decreaseCapturedClusters() {
        capturedClusters--;
    }
};
