

#ifndef SOCKETS_SOCKETADDRESS_H
#define SOCKETS_SOCKETADDRESS_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>
#include <memory>

const int NO_ERROR = 0;

class SocketAddress {
public:
    SocketAddress(uint16_t port, uint32_t address){
        getAsSockIn()->sin_port = htons(port);
        getAsSockIn()->sin_addr.s_addr = htonl(address);
        getAsSockIn()->sin_family = AF_INET;
    }
    SocketAddress(const sockaddr& in_addr){
        memcpy(&myAddr, &in_addr, sizeof(sockaddr));
    }
    SocketAddress()
    {
        getAsSockIn()->sin_family = AF_INET;
        GetIP4Ref() = INADDR_ANY;
        getAsSockIn()->sin_port = 0;
    }

    socklen_t getSize()const{
        return sizeof(sockaddr);
    }
    sockaddr_in* getAsSockIn(){
        return reinterpret_cast<sockaddr_in*>(&myAddr);
    }
private:
    sockaddr myAddr;
    uint32_t&				GetIP4Ref()					{ return getAsSockIn()->sin_addr.s_addr; }
    friend class TCPSocket;
};


#endif //SOCKETS_SOCKETADDRESS_H
