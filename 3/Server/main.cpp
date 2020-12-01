#include "Server.cpp"
#include <csignal>

Server server;

void callDump(int signal) {
    server.dump();
}

int main() {
    signal(SIGINT, callDump);
    string address = "127.0.0.1";
    u_short port = 5223;
    server.launch(address, port);
    return 0;
}