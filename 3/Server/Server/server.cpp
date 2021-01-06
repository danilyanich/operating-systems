#include <winsock2.h> 
#include <windows.h>
#include <winbase.h>

#pragma warning (disable : 4786) 
#pragma warning (disable : 4996)

#include <fstream>
#include <sstream>
#include <vector>
#include <direct.h>
#include <process.h> 
#include <ctime>

#pragma comment(lib, "Ws2_32.lib")

#define MY_PORT 5223		 
#define TIMEOUT 1000

using namespace std;

ostringstream buffer;		 
vector<HANDLE> arrHandle;

struct tagINF {
	sockaddr_in* caddr;
	SOCKET* soc;
	HANDLE hID;
};

#define PRINTUSERS if (numOfClients) printf("%d user on-line\n", numOfClients);\
else printf("No User on line\n");

CRITICAL_SECTION  critSect;


unsigned __stdcall Client(LPVOID info);

int numOfClients = 0;
void deinitialize();
void logToFile(ofstream& logFile, const char* message, int sock);
HWND hwnd;

const char* granted = "GRANTED";
const char* request = "REQUEST";
const char* answer = "ANSWER";
ofstream logFile("logServer.txt", ios::out);

int main(int argc, char* argv[]) {
	HANDLE hStdout;

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY);

	char tmp[1000];			  
	char buff[1024];		  

	printf("TCP SERVER \n");

	if (WSAStartup(0x0202, (WSADATA*)&buff[0]))	{
		printf("ERROR WSAStartup %d\n", WSAGetLastError());
		logToFile(logFile, "ERROR WSAStartup", 0);
		return -1;
	} else {
		logToFile(logFile, "Initialization of winSock library", 0);
	}

	InitializeCriticalSection(&critSect);
	
	SOCKET mysocket;

	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("ERROR socket %d\n", WSAGetLastError());
		logToFile(logFile, "ERROR socket", 0);
	} else {
		logToFile(logFile, "Create socket", 0);
	}

		sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(MY_PORT);
	
	local_addr.sin_addr.s_addr = 0;

	if (bind(mysocket, (sockaddr*)&local_addr, sizeof(local_addr))) {
		printf("ERROR bind %d\n", WSAGetLastError());
		logToFile(logFile, "ERROR bind socket with IP-address and port", 0);
		closesocket(mysocket);
	} else {
		logToFile(logFile, "Bind socket with IP-address and port", 0);
	}


	if (listen(mysocket, 0x100)) {
		printf("ERROR listen %d\n", WSAGetLastError());
		logToFile(logFile, "ERROR listen", 0);
		closesocket(mysocket);
	} else {
		logToFile(logFile, "Listen", 0);
	}

	strcpy_s(tmp, "Ожидание подключений...\n");
	AnsiToOem(tmp, tmp);
	puts(tmp);
	
	SOCKET client_socket;
	
	sockaddr_in client_addr;

	
	int client_addr_size = sizeof(client_addr);

	while ((client_socket = accept(mysocket, (sockaddr*)&client_addr, &client_addr_size))) {
		logToFile(logFile, "Accept client", 0);
		numOfClients++;

		HOSTENT* hst;
		hst = gethostbyaddr((char*)	&client_addr.sin_addr.s_addr, 4, AF_INET);

		PRINTUSERS unsigned  thID;
		tagINF info;

		info.caddr = &client_addr;
		info.soc = &client_socket;

		HANDLE hID = (HANDLE)_beginthreadex(NULL, 0, &Client, &info, 0, &thID);
		arrHandle.push_back(hID);
	}

	deinitialize();
	logFile.close();
	return 0;
}

void deinitialize() {
	logToFile(logFile, "Deinitialize winsock library", 0);
	WSACleanup();
	vector<HANDLE>::iterator it;
	for (it = arrHandle.begin(); it != arrHandle.end(); it++) {
		CloseHandle(*it);
	}
	DeleteCriticalSection(&critSect);
}

unsigned __stdcall Client(LPVOID info) {
	logToFile(logFile, "Create thread for client", 0);
	tagINF inf;
	SOCKET my_sock;
	sockaddr_in my_addr;

	inf = ((tagINF*)info)[0];
	my_sock = *(inf.soc);
	my_addr = *(inf.caddr);

	char buff[1024];
	int bytes_recv;

	while ((bytes_recv = recv(my_sock, buff, sizeof(buff), 0)) && bytes_recv != SOCKET_ERROR) {
		logToFile(logFile, "Recieve message from client ", my_sock);
		EnterCriticalSection(&critSect);
		logToFile(logFile, "ENTER critical section ", my_sock);
		buff[bytes_recv] = 0;

		printf("client %d,  recive string: %s\n", my_sock, buff);
		if (strcmp(buff, request) == 0) {
			logToFile(logFile, "Recieve request from ", my_sock);
			send(my_sock, granted, strlen(granted), 0);
		} else {
			logToFile(logFile, "Recieve function value from ", my_sock);
			ofstream out("file.txt", ios::app);

			if (out.is_open()) {
				char bufferForTime[80];
				time_t seconds = time(NULL);
				tm* timeinfo = localtime(&seconds);
				const char* format = "%A, %B %d, %Y %H:%M:%S";
				strftime(bufferForTime, 80, format, timeinfo);

				out << bufferForTime << " ";
				out << buff << endl;
				logToFile(logFile, "WRITE to file value from ", my_sock);
				out.close();
			}

			Sleep(1000 * 2);
			send(my_sock, answer, strlen(answer), 0);
			logToFile(logFile, "Send answer to ", my_sock);
		}
		LeaveCriticalSection(&critSect);
		logToFile(logFile, "LEAVE critical section ", my_sock);
	}
	closesocket(my_sock);
	logToFile(logFile, "Close my_sock ", my_sock);
	ExitThread(0);
	return 0;
}

void logToFile(ofstream& logFile, const char* message, int sock) {
	char bufferForTime[80];
	time_t seconds = time(NULL);
	tm* timeinfo = localtime(&seconds);
	const char* format = "%A, %B %d, %Y %H:%M:%S";
	strftime(bufferForTime, 80, format, timeinfo);

	if (sock == 0) {
		logFile << bufferForTime << " " << message << std::endl;
	} else {
		logFile << bufferForTime << " " << message << sock << std::endl;
	}
}


