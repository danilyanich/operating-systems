#include <iostream>
#include <string>
#include "memory/soft/System.h"
#include <fstream>
using namespace std;

int main() {
    auto* memory = new PhysicalMemory(100);
    auto* table = new IndexesTable(*memory);
    System system(*table);
    printf("Inited\n");
    while (true) {
        string command;
        //printf("dump|list\ncrf|rf|wf|cpy|mov|del\n");
        cin >> command;
        if (command == "crf") {
            string name;
            cin >> name;
            system.allocateFile(name);
        }
        else if (command == "wf") {
            string name;
            char v;
            cin >> name >> v;
            system.writeToFile(name, v);
        }
        else if (command == "rf") {
            string name;
            cin >> name;
            cout << system.readFile(name) << endl;
        }
        else if (command == "cpy") {
            string from, to;
            cin >> from >> to;
            system.copyFile(from, to);
        }
        else if (command == "del") {
            string name;
            cin >> name;
            system.deleteFile(name);
        }
        else if (command == "mov") {
            string from, to;
            cin >> from >> to;
            system.moveFile(from, to);
        }
        else if (command == "link") {
            string name, link;
            cin >> name >> link;
            system.linkFIle(name, link);
        }
        else if (command == "crd") {
            string path;
            cin >> path;
            ofstream out(path.c_str());
            out << system;
            out.close();
        }
        else if (command == "ld") {
            string path;
            cin >> path;
            ifstream in(path.c_str());
            in >> system;
            in.close();
        }
        else if (command == "list") {
            system.listAllFiles();
        }
        else if (command == "stop")
            break;
    }
    return 0;
}