#pragma once

#include <string>

using namespace std;

class ClientDetails {
private:

    string message;
    bool messageReceived = false;
    bool messageSent = false;
    bool disconnected = false;
    int position = 0;

public:
    const string &getMessage() const {
        return message;
    }

    void setMessage(const string &message) {
        ClientDetails::message = message;
    }

    bool isMessageReceived() const {
        return messageReceived;
    }

    void setMessageReceived(bool messageReceived) {
        ClientDetails::messageReceived = messageReceived;
    }

    bool isMessageSent() const {
        return messageSent;
    }

    void setMessageSent(bool messageSent) {
        ClientDetails::messageSent = messageSent;
    }

    bool isDisconnected() const {
        return disconnected;
    }

    void setDisconnected(bool disconnected) {
        ClientDetails::disconnected = disconnected;
    }

    int getPosition() const {
        return position;
    }

    void setPosition(int position) {
        ClientDetails::position = position;
    }
};