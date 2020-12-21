#include <iostream>
#include <string>

#include "File.h"
#include "Manager.h"

using namespace std;

int main() {
	Manager manager;

	while (true) {
		cout << "1) all files" << endl <<
			"2) create file" << endl <<
			"3) delete file" << endl <<
			"4) write to file" << endl <<
			"5) read file " << endl <<
			"6) copy file " << endl <<
			"7) exit" << endl; 

		int input;
		cin >> input;
		string fileName, content;

		switch (input) {
		case 1:
			manager.printAllFiles();
			break;
		case 2:
			cout << "input new file name:" << endl;
			cin >> fileName;
			manager.createFile(fileName);
			break;

		case 3:
			cout << "input file you want to delete:" << endl;
			cin >> fileName;
			manager.deleteFile(fileName);
			break;

		case 4:
			cout << "input file name:" << endl;
			cin >> fileName;
			cout << "input content:" << endl;
			cin >> content;
			manager.writeFile(fileName, content);
			break;

		case 5:
			cout << "input file name:" << endl;
			cin >> fileName;
			cout << manager.readFile(fileName) << endl;
			break;

		case 6:
			cout << "input file you want to copy:" << endl;
			cin >> fileName;
			manager.copyFile(fileName);
			break;

		case 7:
			exit(0);
			break;

		default:
			break;

		}
	}

	return 0;
}