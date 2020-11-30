#pragma once

#include <string>
#include <Windows.h>

namespace quene {
	class Quene
	{
	public:
		class Element {
			public:
			int threadNum;
			unsigned int nLink;
		};
		typedef Element* pElement;

		Quene(int maxElements);
		~Quene();
		BOOL put(pElement element, DWORD milliseseconds);
		BOOL pull(pElement element);
	private:
		int maxElements;
		HANDLE mutex_semaphore[2];
		HANDLE mutex;
		HANDLE semaphore;
		pElement arrElements;
	};
}

