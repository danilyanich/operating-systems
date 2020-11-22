#pragma once

#include "ReadWriteService.cpp"
#include "DumpService.cpp"
#include "ActionResult.cpp"
#include "InterfaceFileSystem.cpp"
#include "InterfaceCluster.cpp"
#include "ActionResult.cpp"

using namespace std;

class FileSystemService {
private:
    InterfaceFileSystem *interfaceFileSystem;

    InterfaceCluster *interfaceCluster;

    ReadWriteService *readWriteService;

    DumpService *dumpService;
public:

    void setFileSystemDao(InterfaceFileSystem *interfaceFileSystem) {
        this->interfaceFileSystem = interfaceFileSystem;
    }

    void setClusterDao(InterfaceCluster *interfaceCluster) {
        this->interfaceCluster = interfaceCluster;
    }

    void setReadWriteService(ReadWriteService *readWriteService) {
        this->readWriteService = readWriteService;
    }

    void setDumpService(DumpService *dumpService) {
        this->dumpService = dumpService;
    }

    ActionResult *createFile(const string &path) {
        try {
            interfaceFileSystem->save(path, new CustomFile(path));
            return new ActionResult("File is created", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }

    ActionResult *deleteFile(const string &path) {
        try {
            CustomFile *deletedFile = interfaceFileSystem->deleteFile(path);
            interfaceCluster->deleteAllClusters(deletedFile->getAddresses());
            delete deletedFile;
            return new ActionResult("File is deleted", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }

    ActionResult *copyFile(const string &path, const string &copyPath) {
        try {
            CustomFile *file = interfaceFileSystem->findByName(path);
            CustomFile *copy = interfaceFileSystem->save(copyPath, new CustomFile(copyPath));
            readWriteService->fillFile(copy, readWriteService->readClusters(file));
            return new ActionResult("File is copied", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }

    ActionResult *moveFile(const string &path, const string &movePath) {
        return copyFile(path, movePath);
    }

    ActionResult *readFile(const string &path) {
        try {
            CustomFile *file = interfaceFileSystem->findByName(path);
            vector<Cluster *> *clusters = readWriteService->readClusters(file);
            readWriteService->printClusters(clusters);
            delete clusters;
            return new ActionResult("\nFile is read", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }

    ActionResult *writeFile(const string &path, char *data, int size) {
        try {
            CustomFile *file = interfaceFileSystem->findByName(path);
            vector<Cluster *> *clusters = readWriteService->readClusters(file);
            readWriteService->writeClusters(file, data, size, clusters);
            interfaceFileSystem->updateFile(path, file);
            delete clusters;
            return new ActionResult("File is written", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }

    ActionResult *rewriteFile(const string &path, char *data, int size) {
        try {
            deleteFile(path);
            auto *file = new CustomFile(path);
            auto *clusters = new vector<Cluster *>();
            readWriteService->writeClusters(file, data, size, clusters);
            interfaceFileSystem->save(path, file);
            delete clusters;
            return new ActionResult("File is rewritten", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }

    ActionResult *dump() {
        try {
            dumpService->dump();
            return new ActionResult("Dump is printed", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }

    ActionResult *listFiles() {
        try {
            vector<CustomFile *> *files = interfaceFileSystem->findAll();
            for (auto &file : *files) {
                cout << file->getName() << endl;
            }
            return new ActionResult("Files is printed", true);
        } catch (exception &e) {
            return new ActionResult(e.what(), false);
        }
    }
};