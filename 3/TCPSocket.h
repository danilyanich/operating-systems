

#ifndef SOCKETS_TCPSOCKET_H
#define SOCKETS_TCPSOCKET_H

#include "SocketAddress.h"

class TCPSocket {
public:
    int Connect(const SocketAddress& inAdr);
    int Bind(const SocketAddress& inAdr);
    int Listen(int inBackLog = 32);
    std::shared_ptr<TCPSocket> Accept(SocketAddress& inAdr);
    int Send(const void* inDate, int len);
    int Recieve(void* inBuffer, int len);

    static std::shared_ptr<TCPSocket> CreateTCP();
    explicit TCPSocket(int socket):mySocket(socket){};
private:
    int mySocket;
};

typedef std::shared_ptr<TCPSocket> TCPSocketPtr;


#endif //SOCKETS_TCPSOCKET_H
