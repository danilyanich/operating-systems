#pragma once
#include <string>
#include <Windows.h>

class Queue {
public:
	struct Element
	{
		int threadNum;
		unsigned int nLink;
	};
	typedef Element* pElement;
private:
	pElement arrElements;
	int maxElements;
	HANDLE mutex_semaphore[2];
	HANDLE& mutex;
	HANDLE& semaphore;
public:
	Queue(int maxElements);
	~Queue();
	BOOL put(pElement element, DWORD milliseseconds);
	BOOL pull(pElement element);
};