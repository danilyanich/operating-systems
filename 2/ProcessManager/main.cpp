#include <future>
#include <thread>
#include <chrono>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <windows.h>
#include <vector>

using namespace std;
long timeout = 10000;
vector<PROCESS_INFORMATION> processes(10);
BOOL terminate_process_ed(DWORD dwProcessId, UINT uExitCode);
void terminate_by_process(PROCESS_INFORMATION pi);
void CALLBACK timer_callback(
        _In_  PVOID lpParameter,
        _In_  BOOLEAN timer_condition);
int set_async_timer(DWORD pid);
TCHAR s[] = R"(C:\\Windows\\System32\\Notepad.exe)";
//TCHAR s[] = R"(O:\projects\ArgentumServer\cmake-build-debug\multiply_clients.exe)";

void on_exit();
int create_process(unsigned int timeout2);
int main(int argc, char **arg) {

    atexit(on_exit);
    at_quick_exit(on_exit);
    if (argc != 3 + 1) {
        printf("Argument error");
        return -1;
    }
    int i = 0;
    for (i = 0; i < atoi(arg[2]); i++) {
        create_process(atoi(arg[3]));
    }
    this_thread::sleep_for(std::chrono::seconds(200));
    return 0;
}
BOOL terminate_process_ed(DWORD dwProcessId, UINT uExitCode) {
    DWORD dwDesiredAccess = PROCESS_TERMINATE;
    BOOL bInheritHandle = FALSE;
    HANDLE hProcess = OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId);
    if (hProcess == nullptr)
        return FALSE;

    BOOL result = TerminateProcess(hProcess, uExitCode);

    CloseHandle(hProcess);

    return result;
}

void terminate_by_process(PROCESS_INFORMATION pi) {
    terminate_process_ed(pi.dwProcessId, 0);
}

void CALLBACK timer_callback(
        _In_  PVOID lpParameter,
        _In_  BOOLEAN timer_condition) {
    if (timer_condition) {
        auto dwProcessID = reinterpret_cast<DWORD>(lpParameter);
        terminate_process_ed(dwProcessID, 0);
    } else {
        create_process(timeout);
    }
    //std::cout << dwProcessID << std::endl;
}

int set_async_timer(DWORD pid) {
    DWORD dwProcessID = pid;
    HANDLE hProcHandle = OpenProcess(SYNCHRONIZE, FALSE, dwProcessID);
    if (hProcHandle) {
        HANDLE hNewHandle;
        if (RegisterWaitForSingleObject(
                &hNewHandle,
                hProcHandle,
                &timer_callback,
                reinterpret_cast<void *>(dwProcessID),
                10000,
                WT_EXECUTEONLYONCE)) {
            //UnregisterWait(hNewHandle);
        }
        //CloseHandle(hProcHandle);
    }
    return 0;
}

void on_exit() {
    for_each(processes.begin(), processes.end(), terminate_by_process);
}

int create_process(unsigned int timeout2) {
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
        printf("CreateProcess failed (%lu).\n", GetLastError());
        return 1;
    }
    processes.push_back(pi);
    set_async_timer(pi.dwProcessId);
    return 0;
}