// ImageService.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include <cstdlib>
#include <process.h>
#include <chrono>
#include <windows.h>

#include "ImageService.h"
#include "ImageLink.h"
#include "ImageLink.cpp"
#include "Quene.h"
#include "Quene.cpp"
using namespace std;

quene::Quene quene(6);
SOCKET socketArr[1024];
int socketAddressNumber = -1;
string imageNameArr[1024];


int main()
{
	char logFileName[100];
	printf("The programm`re woring now");
	time_t t = time(0);
	tm* tm_now = localtime(&t);
	strftime(logFileName, 80, "%c", tm_now);
	unsigned int qThreadID = 0;
	HANDLE threadsHandler = (HANDLE)_beginthreadex(NULL, 0, runThreadPool, NULL, 0, &qThreadID);
}
SOCKET ImageService::ImageService::establishConnection(string*, string*)
{
	return SOCKET();
}
void ImageService::ImageService::start()
{
	string urlAdress;
	int numLink = -1;
	while (numLink > 100) {
		cin >> urlAdress;
		startDownoloadProcess(&urlAdress, &numLink);
	} 
}

void ImageService::ImageService::startDownoloadProcess(string* url, int* numlink)
{
	ImageLink* link = convertToImageLink(*url, &socketAddressNumber, imageNameArr);
	socketArr[socketAddressNumber] = establishConnection(&link->hostName, &link->path);
	FD_SET(socketArr[socketAddressNumber], &readfds);
	(*numlink)++;
	quene::Quene::Element element = { (*numlink), (*numlink) };
	bool rez = quene.put(&element, 1000);
	if (!rez) {
		printf("Failed to add new element to sync queue\n");
	}
}


void ImageService::ImageService::workTestEvent() 
{
	string urlAdress;
	char link[500];
	int numLink = -1;
	ifstream file("Test/ImageLinkTest.txt");
	if (!file) {
		printf("The program doesn`t found test file");
		return;
	}
	file >> link;
	printf("%s",link);
	getline(file, urlAdress);
	startDownoloadProcess(&urlAdress, &numLink);
	file.close();
}

string ImageService::ImageService::getCurrentTimeString()
{
	time_t seconds = time(NULL);
	tm* tm_sec = localtime(&seconds);
	char timeForAnswer[100];
	strftime(timeForAnswer, 80, "%c", tm_sec);
	return timeForAnswer;
}

unsigned __fastcall ImageService::ImageService::dowloadUrlFile(int* pArg) //
{
	unsigned int numPtr = *pArg;
	unsigned int numberLink = numPtr;
	string timeForAnswer = ImageService::getCurrentTimeString();
	
	return(0);
}

unsigned __fastcall ImageService::ImageService::runThreadPool(int* pArg)
{
	quene::Quene::Element element = {};
	if (quene.pull(&element)) {
		unsigned int* arg;
			arg = (unsigned int*)malloc(sizeof(unsigned int));
			*arg = element.numberLink;
			HANDLE handle = (HANDLE)_beginthreadex(NULL, 1000, dowloadUrlFile, (int*)arg, 0, &element.numberLink);
	}
	else {
		printf("run thread doens`t work\n");
	}
	return 0;
}

