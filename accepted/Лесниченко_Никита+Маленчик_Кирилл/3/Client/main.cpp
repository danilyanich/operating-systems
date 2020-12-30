#include "Client.cpp"

int main() {
    Client client;
    string address = "127.0.0.1";
    u_short port = 5223;
    if (client.connectToServer(address, port)) {
        client.sleep();
        client.sendRequest();
        client.receiveResponse();
        client.sleep();
        client.disconnect();
    }
    return 0;
}