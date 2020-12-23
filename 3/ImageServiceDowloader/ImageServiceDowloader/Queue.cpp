#include "Queue.h"
#include <Windows.h>
#include <vector>

Queue::Queue(int maxElements)
	:mutex(mutex_semaphore[0]), semaphore(mutex_semaphore[1])
{
	arrElements = (pElement)
		HeapAlloc(GetProcessHeap(), 0, sizeof(Element) * maxElements);
	this->maxElements = maxElements;
	mutex = CreateMutex(NULL, FALSE, NULL);
	semaphore = CreateSemaphore(NULL, 0, maxElements, NULL);

}

BOOL Queue::put(pElement element, DWORD milliseseconds)
{
	BOOL fOk = FALSE;
	DWORD dw = WaitForSingleObject(mutex, milliseseconds);

	if (dw == WAIT_OBJECT_0) {
		LONG lPrevCount;
		fOk = ReleaseSemaphore(semaphore, 1, &lPrevCount);
		if (fOk) {
			arrElements[lPrevCount] = *element;
		}
		else {
			SetLastError(ERROR_DATABASE_FULL);
		}
		ReleaseMutex(mutex);
	}
	else {
		SetLastError(ERROR_TIMEOUT);
	}
	return(fOk);

}

BOOL Queue::pull(pElement element) {
	BOOL fOk = (WaitForMultipleObjects(2, mutex_semaphore, TRUE, INFINITE) == WAIT_OBJECT_0);
	if (fOk) {
		*element = arrElements[0];
		MoveMemory(&arrElements[0], &arrElements[1], sizeof(Element) * (maxElements - 1));
		ReleaseMutex(mutex);
	}
	else {
		SetLastError(ERROR_TIMEOUT);
	}
	return(fOk);

}

Queue::~Queue()
{
	CloseHandle(semaphore);
	CloseHandle(mutex);
	HeapFree(GetProcessHeap(), 0, arrElements);
}