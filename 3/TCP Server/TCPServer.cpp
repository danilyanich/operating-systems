#pragma comment(lib, "ws2_32.lib")

#include <iostream>
#include <WinSock2.h>
#include <winsock.h>
#include <thread>
#include <mutex>
#include <WS2tcpip.h>
#include <sstream>

class TCPServer {
private:
	SOCKET serverSocket;
	WSAData serverWSA;
	unsigned short port = 5223;
	std::string serverBuffer = "";
	std::mutex mutx;
	bool isStoped = false;

	void serverThread(SOCKET socket, SOCKADDR_IN clientAddr) {
		struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&clientAddr;
		struct in_addr ipAddr = pV4Addr->sin_addr;
		char ipAddrChar[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipAddr, ipAddrChar, INET_ADDRSTRLEN);
		std::string ipAddrStr = ipAddrChar;

		std::ostringstream ss;
		ss << std::this_thread::get_id();
		std::string threadStr = ss.str();

		std::string str = threadStr + ": accept new client " + ipAddrStr + "\n";
		
		std::unique_lock<std::mutex> lock(mutx);
		serverBuffer += str;
		lock.unlock();

		char buffer[1024] = { 0 };
		recv(socket, buffer, 1024, 0);
		str = buffer;

		lock.lock();
		serverBuffer += threadStr +": " + str + "\n";
		lock.unlock();

		send(socket, buffer, strlen(buffer), 0);

		while (send(socket, buffer, 1, 0) > 0) {
			Sleep(1000); 
			lock.lock();
			serverBuffer += threadStr + ": idle\n";
			lock.unlock();
		}

		lock.lock();
		serverBuffer += threadStr + ": client " + ipAddrStr + " disconected\n";
		lock.unlock();
	}

	void handle() {
		while (!isStoped) {
			Sleep(500);
			SOCKET acceptS;
			SOCKADDR_IN addr_c;
			int addrlen = sizeof(addr_c);
			if ((acceptS = accept(serverSocket, (struct sockaddr*)&addr_c, &addrlen)) != 0) {
				std::thread newServerThread(&TCPServer::serverThread, this, acceptS, addr_c);
				newServerThread.join();
			}
		}
	}
public:
	void startServer() {
		if (WSAStartup(MAKEWORD(2, 2), &serverWSA) == 0) {
			printf("WSA Startup succes\n");
		}

		SOCKADDR_IN addr;
		addr.sin_addr.S_un.S_addr = INADDR_ANY;
		addr.sin_port = htons(port);
		addr.sin_family = AF_INET;

		serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
		if (serverSocket == SOCKET_ERROR) {
			printf("Socket not created\n");
		}

		if (bind(serverSocket, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) {
			printf("Socket succed binded\n");
		}

		if (listen(serverSocket, SOMAXCONN) != SOCKET_ERROR) {
			printf("Start listenin at port %u\n", ntohs(addr.sin_port));
		}

		std::thread handleThread(&TCPServer::handle, this);

		while (true) {
			std::string com;
			std::cin >> com;
			if (com == "INT") {
				std::unique_lock<std::mutex> lock(mutx);
				std::cout << serverBuffer;
				serverBuffer = "";
				lock.unlock();
			}
			if (com == "stop") {
				isStoped = true;
				closeServer();
			}
		}
	}

	void closeServer() {
		closesocket(serverSocket);
		WSACleanup();
		std::cout << "Server was stoped. You can close app" << std::endl;
	}
};