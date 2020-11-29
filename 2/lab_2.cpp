#include "pch.h"
#include <iostream>
#include <windows.h>
#include <vector>

using namespace std;

vector <DWORD> processesID;

int createProcess(LPWSTR path, time_t time) {
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (!CreateProcess(nullptr, path, nullptr, nullptr, 0, 0, nullptr, nullptr, &startInfo, &processInfo))
		return 0;

	processesID.push_back(processInfo.dwProcessId);
	
	return 1;
}

void stopProcess(DWORD id) {
	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, id);
	TerminateProcess(hProcess, 0);
	CloseHandle(hProcess);
}

void stopAll() {
	for (int i = 0; i < processesID.size(); i++)
		stopProcess(processesID[i]);
}


int main()
{
	int copies = 5;
	time_t time = 10000;
	TCHAR path[] = TEXT("C:\\Windows\\System32\\notepad.exe");

	for (int i = 0; i < copies; i++) 
		createProcess(path, time);

	stopAll();
	
}

