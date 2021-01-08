#include "Server.cpp"

Server server;

int main() {
    server.start("127.0.0.1", 5223);
    return 0;
}