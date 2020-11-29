#include "pch.h"

#include <iostream>
#include <windows.h>
#include <vector>
#include <chrono>
#include <thread>

using namespace std;

vector <DWORD> processesID;
int processTime = 10000;
TCHAR path[] = TEXT("C:\\Windows\\System32\\notepad.exe");

int createProcess(LPWSTR path, int time);
void stopProcess(DWORD id);
void CALLBACK timerCallback(_In_  PVOID lpParameter, _In_  BOOLEAN condition);
void runTimer(DWORD processID, int time);
void stopAll();

int main()
{
	int copies = 5;	

	for (int i = 0; i < copies; i++) 
		createProcess(path, processTime);

	atexit(stopAll);
	this_thread::sleep_for(chrono::seconds(15));
}

int createProcess(LPWSTR path, int time) {
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (!CreateProcess(nullptr, path, nullptr, nullptr, 0, 0, nullptr, nullptr, &startInfo, &processInfo))
		return 0;

	processesID.push_back(processInfo.dwProcessId);
	runTimer(processInfo.dwProcessId, time);

	return 1;
}

void stopAll() {
	for (int i = 0; i < processesID.size(); i++)
		stopProcess(processesID[i]);
}

void stopProcess(DWORD id) {
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, id);
	TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
}

void runTimer(DWORD processID, int time) {
	HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, processID);
	if (!hProcess) return;

	HANDLE hNewHandle;
	RegisterWaitForSingleObject(&hNewHandle, hProcess, &timerCallback, reinterpret_cast<void*>(processID), time, WT_EXECUTEONLYONCE);
}

void CALLBACK timerCallback(_In_  PVOID lpParameter, _In_  BOOLEAN condition) {
	if (condition) {
		DWORD dwProcessID = reinterpret_cast<DWORD>(lpParameter);
		stopProcess(dwProcessID);
	}
	else {
		createProcess(path, processTime);
	}
}

