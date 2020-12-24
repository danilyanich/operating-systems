#include "TCPSocket.h"

#include <memory>

int TCPSocket::Connect(const SocketAddress &inAdr) {
    int error = connect(mySocket, &inAdr.myAddr, inAdr.getSize());
    if(error < 0){
        return -error;
    }
    return NO_ERROR;
}

int TCPSocket::Bind(const SocketAddress &inAdr) {
    int error = bind(mySocket, &inAdr.myAddr, inAdr.getSize());
    if(error < 0){
        return -error;
    }
    return NO_ERROR;
}

int TCPSocket::Listen(int inBackLog) {
    int error = listen(mySocket, SOMAXCONN);
    if(error < 0){
        return -error;
    }
    return NO_ERROR;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept(SocketAddress &inAdr) {
    auto len = inAdr.getSize();
    int newSocket = accept(mySocket, &inAdr.myAddr, &len);
    if(newSocket != -1){
        return std::make_shared<TCPSocket>(newSocket);
    }else{
        return nullptr;
    }
}

int TCPSocket::Send(const void *inDate, int len) {
    int bytesSent = send(mySocket, inDate, len, MSG_NOSIGNAL);
    if(bytesSent < 0){
        return -bytesSent;
    }
    return bytesSent;
}

int TCPSocket::Recieve(void *inBuffer, int len) {
    int bytesRec = recv(mySocket, inBuffer, len, 0);
    if(bytesRec < 0){
        return -1;
    }
    return bytesRec;
}

std::shared_ptr<TCPSocket> TCPSocket::CreateTCP()
{
    int err = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (err != -1)
    {
        return std::make_shared<TCPSocket>(err);
    } else
    {
        return nullptr;
    }
}
