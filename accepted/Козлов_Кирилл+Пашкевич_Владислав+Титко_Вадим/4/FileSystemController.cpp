#include <iostream>
#include "FileSystem.cpp"

class FileSystemController {
private:
	FileSystem *fileSystem;

	std::string ADD_FILE = "fcreate";
	std::string SHOW_DIR = "dir";
	std::string ADD_DIR = "dircreate";
	std::string CHANGE_DIR = "cd";
	std::string DELETE_DIR = "dirdelete";
	std::string DELETE_FILE = "filedelete";
	std::string CUT_FILE = "filecut";
	std::string CUT_DIR = "dircut";
	std::string COPY_FILE = "fcopy";
	std::string COPY_DIR = "dircopy";
	std::string PASTE_FILE = "fpaste";
	std::string PASTE_DIR = "dirpaste";
	std::string WRITE = "write";
	std::string READ = "read";
	std::string DUMP = "dump";
	std::string HELP = "help";

	std::vector<std::string> handleActionStr(std::string actionStr) {
		std::vector<std::string> command;
		int start = 0;
		int lenght = 0;
		for (int i = 0; i < actionStr.length(); i++) {
			if (actionStr[i] == ' ' || i == actionStr.length() - 1) {
				if (i == actionStr.length() - 1) lenght++;
				std::string str = actionStr.substr(start, lenght);
				start = i + 1;
				lenght = 0;
				command.push_back(str);
			}
			else lenght++;
		}
		return command;
	}

	void help() {
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, FOREGROUND_BLUE);
		std::cout << ADD_FILE << std::endl;
		std::cout << SHOW_DIR << std::endl;
		std::cout << CHANGE_DIR << std::endl;
		std::cout << DELETE_DIR << std::endl;
		std::cout << DELETE_FILE << std::endl;
		std::cout << CUT_FILE << std::endl;
		std::cout << CUT_DIR << std::endl;
		std::cout << COPY_FILE << std::endl;
		std::cout << COPY_DIR << std::endl;
		std::cout << PASTE_FILE << std::endl;
		std::cout << PASTE_DIR << std::endl;
		std::cout << WRITE << std::endl;
		std::cout << READ << std::endl;
		std::cout << HELP << std::endl;
		std::cout << DUMP << std::endl;
		std::cout << "exit" << std::endl;
	}
public:
	FileSystemController(int memory, int clusterSize) {
		fileSystem = new FileSystem(memory, clusterSize);
	}

	std::string getPath() {
		return fileSystem->getPath();
	}

	void action(std::string actionStr) {
		std::vector<std::string> action = handleActionStr(actionStr);
		if (action[0] == ADD_FILE) fileSystem->createFile(action);
		else if (action[0] == SHOW_DIR) fileSystem->showDirectory();
		else if (action[0] == ADD_DIR) fileSystem->createDirectory(action);
		else if (action[0] == CHANGE_DIR) fileSystem->changeDirectory(action);
		else if (action[0] == DELETE_DIR) fileSystem->deleteDirectory(action);
		else if (action[0] == DELETE_FILE) fileSystem->deleteFile(action);
		else if (action[0] == CUT_FILE) fileSystem->cutFile(action);
		else if (action[0] == CUT_DIR) fileSystem->cutDirectory(action);
		else if (action[0] == COPY_FILE) fileSystem->copyFile(action);
		else if (action[0] == COPY_DIR) fileSystem->copyDirectory(action);
		else if (action[0] == PASTE_FILE) fileSystem->pasteFile();
		else if (action[0] == PASTE_DIR) fileSystem->pasteDirectory();
		else if (action[0] == WRITE) fileSystem->writeFile(action);
		else if (action[0] == READ) fileSystem->readFile(action);
		else if (action[0] == DUMP) fileSystem->dummyDump();
		else if (action[0] == HELP) help();
		else {
			HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
			SetConsoleTextAttribute(handle, FOREGROUND_RED);
			std::cout << "wrong command (use help)\n";
		}
	}
};