#include "Server.cpp"

Server server;

int main() {
    string address = "127.0.0.1";
    int port = 5223;
    server.launch(address, port);
    return 0;
}