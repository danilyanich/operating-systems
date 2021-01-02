
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <iomanip>


#define MEMORY_SIZE 512
#define SUCCESS 0
#define NO_SUCH_FILE 1
#define NOT_READED "^_^"

using namespace std;

struct file {
	string name;
	string type;
	vector<int> indexes;
};

struct catalog {
	vector<file> files;
};

vector<string> memory(MEMORY_SIZE);
catalog root;

void dir() {
	for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
		cout << it->name << '.' << it->type << endl;
	}
}

bool checkForFileName(string fileName, string fileType) {
	for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
		if (it->name == fileName && it->type == fileType) {
			return true;
		}
	}
	return false;
}
void createFile(string fileName, string fileType) {
	if (!checkForFileName(fileName, fileType)) {
		file newFile;
		newFile.name = fileName;
		newFile.type = fileType;
		root.files.push_back(newFile);
	}
}

int writeToFile(string fileName, string fileType, string data) {
	for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
		if (it->name == fileName && it->type == fileType) {
			for (unsigned int i = 0; i < data.size(); i++) {
				string temp;
				if (i += 7 < data.size()) {
					temp = data.substr(i - 1, 8);
					bool isWriten = false;
					for (unsigned int j = 0; j < MEMORY_SIZE; j++) {
						if (memory[j].empty() && !isWriten) {
							memory[j] = temp;
							it->indexes.push_back(j);
							isWriten = true;
						}
					}
				}
				else {
					temp = data.substr(i, data.size());
					bool isWriten = false;
					for (unsigned int j = 0; j < MEMORY_SIZE; j++) {
						if (memory[j].empty() && !isWriten) {
							memory[j] = temp;
							it->indexes.push_back(j);
							isWriten = true;
						}
					}
				}
				i += 6;
			}
			return SUCCESS;
		}
	}	return NO_SUCH_FILE;
}

int writeToFile(file& file, string data) {
	for (unsigned int i = 0; i < data.size(); i++) {
		string temp;
		if (i += 7 < data.size()) {
			temp = data.substr(i - 1, 8);
			bool isWriten = false;
			for (unsigned int j = 0; j < MEMORY_SIZE; j++) {
				if (memory[j].empty() && !isWriten) {
					memory[j] = temp;
					file.indexes.push_back(j);
					isWriten = true;
				}
			}
		}
		else {
			temp = data.substr(i, data.size());
			bool isWriten = false;
			for (unsigned int j = 0; j < MEMORY_SIZE; j++) {
				if (memory[j].empty() && !isWriten) {
					memory[j] = temp;
					file.indexes.push_back(j);
					isWriten = true;
				}
			}
		}
		i += 6;
	}
	return SUCCESS;
}
int renameFile(string fileName, string fileType, string newFileName) {
	if (!checkForFileName(newFileName, fileType)) {
		for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
			if (it->name == fileName && it->type == fileType) {
				it->name = newFileName;
				return SUCCESS;
			}
		}
	}
	return NO_SUCH_FILE;
}
string readFromFile(string fileName, string fileType) {
	string temp = NOT_READED;
	for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
		if (it->name == fileName && it->type == fileType) {
			temp.clear();
			for (vector<int>::iterator index = it->indexes.begin(); index < it->indexes.end(); index++) {
				temp += memory[*index];
			}
			return temp;
		}
	}
}
void dump(string dumpName) {
	ofstream dump(dumpName, ios::binary | ios::out);
	dump << "root>" << endl;
	for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
		dump << "	" << it->name << '.' << it->type << endl;
		dump << "	" << "----------------" << endl;
		dump << "	" << "| indexes      |" << endl;
		dump << "	" << "----------------" << endl;
		for (vector<int>::iterator index = it->indexes.begin(); index < it->indexes.end(); index++) {
			dump << "	" << "|     "; dump << setw(3) << *index; dump << "      |" << endl;
		}
		dump << "	" << "----------------" << endl;
		dump << "	" << "Info in file:\n	" << readFromFile(it->name, it->type) << endl;
		dump << endl;
	}
	dump << "-----------------------" << endl;
	dump << "|  Memory table state |" << endl;
	dump << "-----------------------" << endl;
	for (unsigned int i = 0; i < memory.size(); i++) {
		dump << "|"; dump << setw(10) << i; dump << "|"; dump << setw(10) << memory[i]; dump << "|" << endl;
		dump << "-----------------------" << endl;
	}
}
int main()
{
	string request;
	while (true) {
		cout << "\n>";
		cin >> request;
		request = request.substr(0, request.find(' '));
		if (request == "cr") {
			string fileName_dot_Type;
			cin >> fileName_dot_Type;
			createFile(fileName_dot_Type.substr(0, fileName_dot_Type.find('.')),
				fileName_dot_Type.substr(fileName_dot_Type.find('.') + 1, fileName_dot_Type.size()));
		}

		else if (request == "dir") {
			dir();
		}
		else if (request == "write") {
			string fileName_dot_Type, input;
			cin >> fileName_dot_Type;
			cin >> input;
			if (writeToFile(fileName_dot_Type.substr(0, fileName_dot_Type.find('.')),
				fileName_dot_Type.substr(fileName_dot_Type.find('.') + 1, fileName_dot_Type.size()), input) == NO_SUCH_FILE) {
				cout << "Can't write to file. No such file" << endl;
			}
		}
		else if (request == "read") {
			string fileName_dot_Type, output;
			cin >> fileName_dot_Type;
			output = readFromFile(fileName_dot_Type.substr(0, fileName_dot_Type.find('.')),
				fileName_dot_Type.substr(fileName_dot_Type.find('.') + 1, fileName_dot_Type.size()));
			if (output == NOT_READED) {
				cout << "Can't read. No such file" << endl;
			}
			else {
				cout << output;
			}
			cout << endl;
		}
			else if (request == "move") {
			string fileName_dot_Type, newFileName;
			cin >> fileName_dot_Type;
			cin >> newFileName;
			if (renameFile(fileName_dot_Type.substr(0, fileName_dot_Type.find('.')),
				fileName_dot_Type.substr(fileName_dot_Type.find('.') + 1, fileName_dot_Type.size()),
				newFileName) == NO_SUCH_FILE) {
				cout << "Can't rename file" << endl;
			}
		}
		else if (request == "dump") {
			string dumpName;
			cin >> dumpName;
			dump(dumpName);
		}

		else if (request == "help") {
			cout << "dir	Show all files in directory" << endl;
			cout << "cr	create file" << endl;
			cout << "rm	remove file" << endl;
			cout << "move	rename file" << endl;
			cout << "copy	copy file" << endl;
			cout << "write	write data to file" << endl;
			cout << "read	read data from file" << endl;
			cout << "dump	create dump" << endl;
			cout << "cls	clear screen" << endl;
			cout << "exit	finish program" << endl;
		}
		else if (request == "exit") {
			return 0;
		}
		else if (request == "cls") {
			system("cls");
		}
		else {
			cout << "Unknown command..." << endl;
		}
	}
}

