#include "Client.cpp"

int main() {
    Client client;

    if (client.connectTo("127.0.0.1", 5223)) {
        client.sleep();
        client.sendMessage();
        client.getResponse();
        client.sleep();
        client.disconnect();
    }
}