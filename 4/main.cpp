#include <iostream>
#include <fstream>
#include "PhMemory.h"
#include "ControlM.h"
#include "Manager.h"

using namespace std;

int main()
{
    
    auto *phMemory= new PhMemory(50);
    auto *controlM = new ControlM(*phMemory);
    auto manager = new Manager( *controlM);

    int swit;
    bool ex = true;
    while (ex) {
        cout << "1- create file\n2-write to file\n3-read from file\n4-delete file\n5-copy file\n6-move file\n7-list files\n8-create dump\n9-load\n10-exit\n";
        cin >> swit;

        switch (swit)
        {
        case 1: {
            string name;
            cin >> name;
            manager->allocateFile(name);
        }break;
        case 2: {
            string name;
            char v;
            cin >> name >> v;
            manager->writeToFile(name, v);
        }break;
        case 3: {
            string name;
            cin >> name;
            cout << manager->readFile(name) << endl;
        } break;
        case 5: {
            string from, to;
            cin >> from >> to;
            manager->copyFile(from, to);
        }break;
        case 4: {
            string name;
            cin >> name;
            manager->deleteFile(name);
        }break;
        case 6: {
            string from, to;
            cin >> from >> to;
            manager->moveFile(from, to);
        }
        case 7: {
            manager->listAllFiles();
        }break;
        case 8: {
            string path;
            cin >> path;
            ofstream fout(path.c_str());
            fout << manager;
            fout.close();
        }break;
        case 9: {
            string path;
            cin >> path;
            ifstream fin(path.c_str());
            fin >> *manager;
            fin.close();
        }break;
        case 10:ex = false;
            break;
        }
    }
    
    return 0;
}

