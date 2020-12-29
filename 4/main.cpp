#include "command.h"

int main() {
    while(true) {
        cout<<"> ";
        char command[2000];
        cin.clear();
        cin.getline(command, 2000);
        cin.clear();
        startCommand(command);
    }
}