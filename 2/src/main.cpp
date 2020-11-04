#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdio>
#include "atlstr.h"
#include <cstdlib>
#include <windows.h>
#include <vector>

using namespace std;

TCHAR s[] = TEXT("C://Program Files (x86)/Notepad++/notepad++.exe");
vector<PROCESS_INFORMATION> process_info;

int CreateMyProcess(int);

int main(int argc, char* argv[])
{

    /* if (argc != 3 ) {
         printf("Argument error");
         return -1;
     }

     for (int i = 0; i < atoi(argv[1]); i++) {
         CreateProcess(atoi(argv[2]));
     }*/

    for (int i = 0; i < 10; i++) {
        CreateMyProcess(10);
    }
}

int CreateMyProcess(int time) {




    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(nullptr,
        s,
        nullptr,
        nullptr,
        FALSE,
        FALSE,
        nullptr,
        nullptr,
        &si,
        &pi)
        ) {
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }
    process_info.push_back(pi);
    return 0;
}