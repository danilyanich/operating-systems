#include <iostream>
#include <string>

#include "memory/memory.h"

using namespace std;

int main(int argc, char **argv) {
	initfs();
	string command;
	string name;

	while (true) {
		cout << "root@root: ~\n$ ";

		cin >> command;
		if (command == "touch") {
			cin >> name;
			touch(name);
		}
		else if (command == "delete") {
			cin >> name;
			delete_file(name);
		}
		else if (command == "rename") {
			cin >> name;
			string new_name;
			cin >> new_name;
			rename(name, new_name);
		}
		else if (command == "copy") {
			cin >> name;
			copy(name);
		}
		else if (command == "write") {
			string data;
			cin >> name;
			getline(cin, data);
			data = data.substr(1);
			write(name, data);
		}
		else if (command == "read") {
			cin >> name;
			read(name);
		}
		else if (command == "ls") {
			ls();
		}
		else if (command == "exit") {
			break;
		}
	}

	return 0;
}
