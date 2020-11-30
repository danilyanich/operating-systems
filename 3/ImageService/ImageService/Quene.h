#pragma once

#include <string>
#include <Windows.h>

namespace quene {
	class Quene
	{
	private:
		int maxElements;
		HANDLE mutex_semaphore[2];
		HANDLE mutex;
		HANDLE semaphore;
		PELEMENT_TYPE arrElements;
	public:
		Quene(int maxElements);
		~Quene();
		BOOL put(PELEMENT_TYPE element, DWORD milliseseconds);
		BOOL pull(PELEMENT_TYPE element);
		class Element {
		public:
			int threadNum;
			unsigned int nLink;
		};
	};
}

