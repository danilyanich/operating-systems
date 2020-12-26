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

TCHAR s[] = TEXT("C:\\Windows\\System32\\notepad.exe");
vector<PROCESS_INFORMATION> process_info;

int create_my_process(int);
void stop_processes();

int main(int argc, char* argv[])
{
    if (argc == 1) {
        for (int i = 0; i < 10; i++) {
        create_my_process(10000);
        }
    }
    else  if (argc == 3 ) {
        for (int i = 0; i < atoi(argv[1]); i++) {
            create_my_process(atoi(argv[2]));
        }
    }
    else {
        printf("Argument error");
        return -1;
    }

    atexit(stop_processes);
    sleep_for(chrono::seconds(20));
}

void stop_process_by_id(DWORD id) {

    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, id);
    TerminateProcess(hProcess,0);
    CloseHandle(hProcess);
}

void stop_processes() {

    for (int i = 0; i < process_info.size(); i++) {
        stop_process_by_id(process_info[i].dwProcessId);
    }
}

void CALLBACK timer_callback(_In_  PVOID lpParameter,_In_  BOOLEAN timer_condition) {
    
    if (timer_condition) {
        DWORD dwProcessID = reinterpret_cast<DWORD>(lpParameter);
       // cout << dwProcessID << endl;
        stop_process_by_id(dwProcessID);
    }
    else {
      //  cout << "is closed" << endl;
        create_my_process(5000);
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
            10000,
            WT_EXECUTEONLYONCE);
    }
}

int create_my_process(int time) {
  
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
        ){
        printf("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }
    process_info.push_back(pi);
    run_timer(pi,time);
    return 0;
}