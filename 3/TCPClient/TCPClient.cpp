#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>
#include <string>

class TCPClient {
private:
	WSAData clientWSA;
	unsigned short port = 5223;
	std::mutex mutx;

	void connectToServer(int clientNumber) {
		SOCKET clientSocket;
		
		clientSocket = socket(AF_INET, SOCK_STREAM, NULL);
		if (clientSocket == SOCKET_ERROR) {
			std::unique_lock<std::mutex> lock(mutx);
			std::cout << "ERROR: Client socket not created\n";
			lock.unlock();
		}

		std::unique_lock<std::mutex> lock(mutx);
		std::cout << "Client " << clientNumber << " online\n";
		lock.unlock();

		SOCKADDR_IN serverAddr;

		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);

		if (inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr) <= 0)
		{
			lock.lock();
			std::cout << "ERROR: Invalid address/ Address not supported\n";
			lock.unlock();
			return;
		}

		if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
		{
			lock.lock();
			std::cout << "ERROR: Connection Failed\n";
			lock.unlock();
			return;
		}

		Sleep(rand() % 2000 + 5000);

		lock.lock();
		std::cout << "Client " << clientNumber << " send message\n";
		lock.unlock();

		std::string strString = "Hello to server from client " + std::to_string(clientNumber);
		char* strChar;
		strChar = &strString[0];
		send(clientSocket, strChar, strlen(strChar), 0);

		char buffer[1024] = { 0 };
		recv(clientSocket, buffer, 1024, 0);

		lock.lock();
		std::cout << "Client " << clientNumber << " receive message: " << buffer << "\n";
		lock.unlock();

		Sleep(rand() % 2000 + 5000);

		lock.lock();
		std::cout << "Client " << clientNumber << " offline\n";
		lock.unlock();

		closesocket(clientSocket);
	}
public:
	TCPClient() {
		if (WSAStartup(MAKEWORD(2, 2), &clientWSA) == 0) {
			std::cout << "WSA Startup succes\n";
		}
	}

	void start() {
		for (int i = 0; i < 10; i++) {
			std::thread newThread(&TCPClient::connectToServer, this, i + 1);
			newThread.join();
		}
	}
};