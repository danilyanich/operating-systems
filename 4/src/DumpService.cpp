#pragma once

#include<vector>
#include<iostream>
#include "InterfaceFileSystem.cpp"
#include "InterfaceCluster.cpp"

using namespace std;

class DumpService {
private:
    InterfaceCluster *interfaceCluster;
public:

    void setClusterDao(InterfaceCluster *interfaceCluster) {
        this->interfaceCluster = interfaceCluster;
    }

    void dump() {
        vector<Cluster *> *clusters = interfaceCluster->findAll();
        for (unsigned long address = 0; address < clusters->size(); address++) {
            printCluster(clusters->at(address), address);
        }
        delete clusters;
    }

private:
    void printCluster(Cluster *cluster, unsigned long address) {
        printf("%4lu : ", address);
        if (cluster != nullptr) {
            printf("%16s: ", cluster->getFile()->getName().c_str());
            for (int symbolIndex = 0; symbolIndex < cluster->getSize(); symbolIndex++) {
                cout << cluster->getData()[symbolIndex];
            }
        } else {
            cout << "null";
        }
        cout << endl;
    }
};
