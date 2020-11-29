
#include <iostream>
#include "scr/PhMemory.h"
#include "scr/ControlM.h"
#include "scr/Manager.h"

using namespace std;

int main()
{
    
    auto *phMemory= new PhMemory(50);
    auto *controlM = new ControlM(*phMemory);
    auto manager = new Manager( *controlM);

    

    while (true) {
        string command;
      
        cin >> command;
        if (command == "crf") {
            string name;
            cin >> name;
            manager->allocateFile(name);
        }
        else if (command == "wf") {
            string name;
            char v;
            cin >> name >> v;
            manager->writeToFile(name, v);
        }
        else if (command == "rf") {
            string name;
            cin >> name;
            cout << manager->readFile(name) << endl;
        }
        else if (command == "cpy") {
            string from, to;
            cin >> from >> to;
            manager->copyFile(from, to);
        }
        else if (command == "del") {
            string name;
            cin >> name;
            manager->deleteFile(name);
        }
        else if (command == "mov") {
            string from, to;
            cin >> from >> to;
            manager->moveFile(from, to);
        }
        else if (command == "list") {
            manager->listAllFiles();
        }
        else if (command == "stop")
            break;
    }
   
    
    return 0;
}

