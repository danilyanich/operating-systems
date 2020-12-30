#pragma once
#include <string>
#include <Windows.h>

class Queue {
public:
	class Element {
	public:
		int threadNum;
		unsigned int numberLink;
	};
	typedef Element* pElement;

	Queue(int numElements);
	~Queue();
	BOOL put(pElement element, DWORD milliseseconds);
	BOOL pull(pElement element);
private:
	int maxElements;
	HANDLE mutex_semaphore[2];
	HANDLE mutex;
	HANDLE semaphore;
	pElement arrElements;
};