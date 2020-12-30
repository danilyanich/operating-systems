
#include <stdio.h>
#include <windows.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <vector>

PROCESS_INFORMATION CreateNote(int);

int main()
{
	setlocale(LC_ALL, "rus");
	int ThreadNum,time;
	std::string answer;
	std::vector<PROCESS_INFORMATION> threads;
	std::cout << "Введите количество потоков"<<std::endl;
	std::cin >> ThreadNum;
	std::cout << "Введите время работы приложений в милисекундах" << std::endl;
	std::cin >> time;
	for (auto i = 0; i < ThreadNum; i++) {
		threads.push_back(CreateNote(time));
	}
	answer = "y";
	while (answer == "y") {
		std::this_thread::sleep_for(std::chrono::milliseconds(time));	
		std::cout << "Желаете продолжить?" << std::endl;
		std::cin >> answer;
	}
	for (auto i : threads) {
		TerminateProcess(i.hProcess, NO_ERROR);
	}
}

PROCESS_INFORMATION CreateNote(int time) {
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	LPCTSTR s = L"c://windows//notepad.exe";
	if (CreateProcess(s, NULL,
		NULL, NULL, FALSE, IDLE_PRIORITY_CLASS, NULL, NULL, &cif, &pi) == TRUE)
	{
		std::cout << "process" << std::endl;
		std::cout << "handle " << pi.hProcess << std::endl;
		return pi;
	}
}