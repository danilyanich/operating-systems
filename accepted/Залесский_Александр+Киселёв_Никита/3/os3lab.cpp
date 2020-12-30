#define _CRT_SECURE_NO_WARNINGS   
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_NO_VA_START_VALIDATION
#pragma comment(lib, "ws2_32.lib")

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <winsock2.h>
#include <thread>
#include <vector>
#include <Windows.h>
#include <regex>
#include <process.h>
#include "Constants.h"
#include "Queue.h"
#include "ImageLink.h"

using namespace std;
SOCKET socketArr[BUFF_SIZE];
string imageNameArr[BUFF_SIZE];
fstream image[BUFF_SIZE];
fd_set readfds;
int socketAddressNumber = -1;
char logFileName[FILE_NAME_LENGTH];
CRITICAL_SECTION console;
CRITICAL_SECTION file;
Queue queue(5);
vector<HANDLE> activeHandles;

SOCKET establishConnection(string*, string*);
unsigned __stdcall downloadImg(void* pArg);
string format(const string& format, ...);
unsigned __stdcall runThreadPool(void* pArg);
void writeToConsole(CRITICAL_SECTION* section, string message, ostream* target);
void writeToLogFile(string stringToWrite);
void start();
void runTests();
void logInfo(const char* messageToWrite);
void startDownoloadProcess(string* url, int* numlink);

int main(int argc, char* argv[]) {
	printf(START_PROGRAM_MESSAGE);
	time_t t = time(0);
	struct tm* now = localtime(&t);
	strftime(logFileName, 80, LOG_FILE_PATTERN, now);
	CreateDirectoryA(IMAGE_DIRECTORY, 0);
	InitializeCriticalSection(&console);
	InitializeCriticalSection(&file);
	unsigned int qThreadID = 0;
	HANDLE threadsHandler = (HANDLE)_beginthreadex(NULL, 0, runThreadPool, NULL, 0, &qThreadID);
	if (strcmp(argv[1], TEST_MODE) == 0) {
		runTests();
	}
	else {
		start();
	}
	system("pause");
	return 0;
}

void start() {
	string url;
	int numLink = -1;
	while (true) {
		cin >> url;
		startDownoloadProcess(&url, &numLink);
	}
}

void runTests() {
	int numLink = -1;
	string url;
	ifstream link(TEST_FILE);
	if (!link) {
		logInfo(NO_FILE_FOR_TEST);
		return;
	}
	while (getline(link, url)) {
		startDownoloadProcess(&url, &numLink);
	}
	link.close();
}

void startDownoloadProcess(string* url, int* numlink) {

	ImageLink* link = convertToImageLink(*url, &socketAddressNumber, imageNameArr);
	socketArr[socketAddressNumber] = establishConnection(&link->hostName, &link->imagePath);
	FD_SET(socketArr[socketAddressNumber], &readfds);
	(*numlink)++;
	Queue::Element element = { (*numlink), (*numlink) };
	bool rez = queue.put(&element, 1000);
	if (!rez) {
		logInfo(ADD_ELEMENT_IN_QUEUE_EXCEPTION_MESSAGE);
	}
}


unsigned __stdcall runThreadPool(void* pArg)
{
	while (true)
	{
		Queue::Element element = {};
		if (queue.pull(&element)) {
			unsigned int* arg;
			arg = (unsigned int*)malloc(sizeof(unsigned int));
			*arg = element.nLink;
			HANDLE handle = (HANDLE)_beginthreadex(NULL, 1000, downloadImg, (void*)arg, 0, &element.nLink);
			activeHandles.push_back(handle);
		}
	}
	return 0;
}

SOCKET establishConnection(string* host, string* imagePath)
{
	WSADATA wd;
	SOCKET sock;
	struct hostent* hostInfo;

	int result = WSAStartup(MAKEWORD(2, 2), &wd);
	if (result != 0) {
		logInfo(WSDATA_LOAD_EXCEPTION_MESSAGE);
		return result;
	}

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		logInfo(SOCKET_INITIALIZATION_EXCEPTION_MESSAGE);
		return sock;
	}

	sockaddr_in sai = { AF_INET };
	sai.sin_family = AF_INET;

	hostInfo = gethostbyname((*host).c_str());
	if (hostInfo == NULL) {
		logInfo(INCORRECT_LINK_EXCEPTION_MESSAGE);
		return sock;
	}
	sai.sin_port = htons(80);
	memcpy(&sai.sin_addr, hostInfo->h_addr, 4);

	int check = connect(sock, (sockaddr*)&sai, sizeof(sai));
	if (check == SOCKET_ERROR) {
		logInfo(CONNECTION_EXCEPTION_MESSAGE);
		return sock;
	}

	char* requestMessage = new char[sizeof(imagePath) + sizeof(host) + REQUEST_MESSAGE_LENGTH];
	sprintf(requestMessage, REQUEST_MESSAGE, (*imagePath).c_str(), (*host).c_str());
	int sendInfo = send(sock, requestMessage, strlen(requestMessage), 0);
	if (SOCKET_ERROR == sendInfo) {
		logInfo(SEND_REQUEST_EXCEPTION_MESSAGE);
		closesocket(sock);
		return sock;
	}
	delete[] requestMessage;
	return sock;
}

string getCurrentTImeString() {
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	char timeForAnswer[100];
	strftime(timeForAnswer, 80, TIME_TEMPLATE_IN_FILE, timeinfo);
	return timeForAnswer;
}

string generateMessageOfImageDownoloading(string image, int bites) {
	string timeForAnswer = getCurrentTImeString();
	char answer_char[200];
	string answer;

	if (bites > 0) {
		char message[1000];
		sprintf(message, IMAGE_STILL_DOWNLOADING_MESSAGE, timeForAnswer.c_str(), image.c_str(), bites);
		answer = message;
	}
	else if (bites < 0) {
		char message[1000];
		sprintf(message, IMAGE_NOT_DOWNLOADING_MESSAGE, timeForAnswer.c_str(), image.c_str());
		answer = message;
	}
	else {
		char message[1000];
		sprintf(message, IMAGE_DOWNLOAD_SUCCESSFULLY_MESSAGE, timeForAnswer.c_str(), image.c_str());
		answer = message;
	}

	for (int j = 0; j < answer.length(); j++) {
		answer_char[j] = answer[j];
	}

	answer_char[answer.length()] = '\0';
	return answer_char;
}

unsigned __stdcall downloadImg(void* pArg)
{
	unsigned int numPtr = *((int*)pArg);
	unsigned int numLink = numPtr;
	string timeForAnswer = getCurrentTImeString();
	int threadId = std::hash<std::thread::id>{}(std::this_thread::get_id());
	string message = format(START_DOWNOLOAD_IMAGE_MESSAGE, timeForAnswer.c_str(), threadId, imageNameArr[numLink].c_str());
	logInfo(message.c_str());
	char buf[10240];
	image[numLink].open(IMAGE_DIRECTORY + imageNameArr[numLink], ios::out | ios::binary);
	memset(buf, 0, sizeof(buf));
	int n = recv(socketArr[numLink], buf, sizeof(buf) - 1, 0);
	if (n == -1) {
		logInfo(SOCKER_DATA_READ_EXCEPTION_MESSAGE);
		free(pArg);
		_endthreadex(0);
		return 0;
	}
	else {
		char* cpos = strstr(buf, RESPONSE_DELIMETER);
		image[numLink].write(cpos + strlen(RESPONSE_DELIMETER), n - (cpos - buf) - strlen(RESPONSE_DELIMETER));
		string message = generateMessageOfImageDownoloading(imageNameArr[numLink], n);
		writeToLogFile(message);
	}

	while (true) {
		if (FD_ISSET(socketArr[numLink], &readfds)) {
			memset(buf, 0, sizeof(buf));
			n = recv(socketArr[numLink], buf, sizeof(buf) - 1, 0);
			if (n > 0) {
				image[numLink].write(buf, n);
				string message = generateMessageOfImageDownoloading(imageNameArr[numLink], n);
				writeToLogFile(message);
			}
			else if (n == 0) {
				FD_CLR(socketArr[numLink], &readfds);
				image[numLink].close();
				string message = generateMessageOfImageDownoloading(imageNameArr[numLink], n);
				logInfo(message.c_str());
				break;
			}
			else if (n == -1) {
				logInfo(SOCKER_DATA_READ_EXCEPTION_MESSAGE);
			}
		}
	}
	free(pArg);
	_endthreadex(0);
	return(0);
}

void writeToConsole(CRITICAL_SECTION* section, string message, ostream* target) {
	EnterCriticalSection(section);
	*target << message << endl;
	LeaveCriticalSection(section);
}

void writeToLogFile(string stringToWrite)
{
	EnterCriticalSection(&file);
	FILE* fileToSave = fopen(logFileName, "a");
	fprintf(fileToSave, "%s\n", stringToWrite.c_str());
	fflush(fileToSave);
	fclose(fileToSave);
	LeaveCriticalSection(&file);
}

void logInfo(const char* messageToWrite) {
	writeToLogFile(messageToWrite);
	writeToConsole(&console, messageToWrite, &cout);
}

string format(const string& format, ...)
{
	va_list args;
	va_start(args, format);
	size_t len = std::vsnprintf(NULL, 0, format.c_str(), args);
	va_end(args);
	std::vector<char> vec(len + 1);
	va_start(args, format);
	std::vsnprintf(&vec[0], len + 1, format.c_str(), args);
	va_end(args);
	return &vec[0];
}
