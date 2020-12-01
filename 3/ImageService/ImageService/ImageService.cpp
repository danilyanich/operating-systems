// ImageService.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <chrono>

#include "ImageService.h"
#include "ImageLink.h"
#include "ImageLink.cpp"
#include "Quene.h"
#include "Quene.cpp"


int main()
{
	printf("The programm`re woring now");
	
}

void ImageService::ImageService::start()
{
	string url;
	int numLink = -1;
	while (numLink > 100) {
		cin >> url;
		startDownoloadProcess(&url, &numLink);
	} 
}

void ImageService::ImageService::startDownoloadProcess(string* url, int* numlink)
{

}


void ImageService::ImageService::workTestEvent()
{
	string url;
	char link[500];
	int numLink = -1;
	ifstream file("Test/ImageLinkTest.txt");
	if (!file) {
		printf("The program doesn`t found test file");
		return;
	}
	file >> link;
	printf("%s",link);
	getline(file, url);
	startDownoloadProcess(&url, &numLink);
	file.close();
}

string ImageService::ImageService::getCurrentTimeString()
{
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	char timeForAnswer[100];
	strftime(timeForAnswer, 80, "%Y-%m-%d %H:%M:%S", timeinfo);
	return timeForAnswer;
}
