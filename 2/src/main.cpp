#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <vector>

using namespace std;
using namespace this_thread;

//TCHAR s[] = TEXT("C:\\Windows\\System32\\calc.exe");
TCHAR s[] = TEXT("C:\\Windows\\System32\\notepad.exe");
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
        CreateMyProcess(10000);//10s
    }

    sleep_for(chrono::seconds(15));
}
void CALLBACK timer_callback(
    _In_  PVOID lpParameter,
    _In_  BOOLEAN timer_condition) {

    if (timer_condition) {
        DWORD dwProcessID = reinterpret_cast<DWORD>(lpParameter);
        cout << dwProcessID << endl;

    }
    else {
        cout << "is closed" << endl;
        CreateMyProcess(5000);
    }

}
void run_timer(PROCESS_INFORMATION pi, int time) {

    HANDLE ph = OpenProcess(SYNCHRONIZE, FALSE, pi.dwProcessId);
    if (ph) {
        HANDLE hNewHandle;
        RegisterWaitForSingleObject(
            &hNewHandle,
            ph,
            &timer_callback,
            reinterpret_cast<void*>(pi.dwProcessId),
            5000,
            WT_EXECUTEONLYONCE);
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
    run_timer(pi, time);
    return 0;
}