#include "pch.h"
#include <iostream>
#include <windows.h>

using namespace std;

int createProcess(LPWSTR path, time_t time) {
	STARTUPINFO startInfo;
	PROCESS_INFORMATION processInfo;

	ZeroMemory(&startInfo, sizeof(startInfo));
	startInfo.cb = sizeof(startInfo);
	ZeroMemory(&processInfo, sizeof(processInfo));

	if (!CreateProcess(nullptr, path, nullptr, nullptr, 0, 0, nullptr, nullptr, &startInfo, &processInfo))
		return 0;
	
	return 1;
}

void stopProcess() {

}


int main()
{
	int copies = 5;
	time_t time = 10000;
	TCHAR path[] = TEXT("C:\\Windows\\System32\\notepad.exe");

	for (int i = 0; i < copies; i++) 
		createProcess(path, time);
	
}

