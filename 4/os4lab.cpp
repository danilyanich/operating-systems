#include <iostream>
#include <vector>
#include <assert.h>
#include <map>
#include <functional>
#include <algorithm>
#include "FileSystem.h"
#include "ErrorsCode.h"
#include "Jingle.h"

FileSystem *fileSystem;
bool shellState = true;

typedef std::function<int(std::vector<std::string>)> functionPtr;

void wrongFileNameTouchTest(FileSystem *fileSystem) {
	int error = fileSystem->createFile("dsfsdfsdfsdfsdfsdfsdf");
	assert(error == -2);
}

void correctTouchTest(FileSystem *fileSystem) {
	int error = fileSystem->createFile("test");
	assert(error == 0);
}

void fileAlreadyExistTouchTest(FileSystem *fileSystem) {
	int error = fileSystem->createFile("test");
	assert(error == -4);
}

void lackOfMemoryTouchTest(FileSystem *fileSystem) {
	int error = fileSystem->createFile("test1");
	assert(error == 0);
	error = fileSystem->createFile("test2");
	assert(error == -1);
}

void touchTest() {
	FileSystem *fileSystem = new FileSystem();
	fileSystem->initialize(80);
	wrongFileNameTouchTest(fileSystem);
	correctTouchTest(fileSystem);
	fileAlreadyExistTouchTest(fileSystem);
	lackOfMemoryTouchTest(fileSystem);
	delete fileSystem;
	std::cout << "touch tests passed\n";
}

void wrongFileNameWriteTest(FileSystem *fileSystem) {
	int error = fileSystem->write("testsdgdsgsdgsdgsd", "ddsgdsgdsgdfddsgdsgdsgdf");
	assert(error == -2);
}

void fileNotFoundWriteTest(FileSystem *fileSystem) {
	int error = fileSystem->write("test1", "ddsgdsgdsgdfddsgdsgdsgdf");
	assert(error == -3);
}

void correctWriteTest(FileSystem *fileSystem) {
	int error = fileSystem->write("test", "ddsgdsgdsgdfddsgdsgdsgdf");
	assert(error == 0);
}

void lackOfMemoryWriteTest(FileSystem *fileSystem) {
	int error = fileSystem->write("test", "ddsgdsgdsgdfddsgdsgdsgdf");
	assert(error == -1);
}

void writeTest() {
	int error;
	FileSystem *fileSystem = new FileSystem();
	fileSystem->initialize(80);
	fileSystem->createFile("test");
	wrongFileNameWriteTest(fileSystem);
	fileNotFoundWriteTest(fileSystem);
	correctWriteTest(fileSystem);
	lackOfMemoryWriteTest(fileSystem);

	delete fileSystem;

	std::cout << "write test passed\n";
}

void correctInit(FileSystem *fileSystem) {
	int error = fileSystem->initialize(8000);
	assert(error == 0);
}

void initTest() {
	int error = 0;
	FileSystem *fileSystem = new FileSystem();
	correctInit(fileSystem);
	correctInit(fileSystem);
	delete fileSystem;
	fileSystem = new FileSystem();
	correctInit(fileSystem);
	delete fileSystem;
	std::cout << "init tests passed\n";

}

void wrongNameReadTest(FileSystem *fileSystem) {
	std::string str;
	str = fileSystem->read("gfdgdfgfdgggggggggggg");
	assert(str == "[ERROR] WRONG FILE NAME\a");
}

void fileNotFoundReadTest(FileSystem *fileSystem) {
	std::string str;
	str = fileSystem->read("test");
	assert(str == "[ERROR] FILE NOT FOUND\a");
}

void readEmptyFileTest(FileSystem *fileSystem) {
	std::string str;
	str = fileSystem->read("test");
	assert(str == "");
}

void readNotEmptyFileTest(FileSystem *fileSystem) {
	std::string str;
	str = fileSystem->read("test");
	assert(str == "ddsgdsgdsgdfddsgdsgdsgdf");
}

void readTest() {
	std::string str;
	int error;
	FileSystem *fileSystem = new FileSystem();
	error = fileSystem->initialize(80);
	wrongNameReadTest(fileSystem);
	fileNotFoundReadTest(fileSystem);
	fileSystem->createFile("test");
	readEmptyFileTest(fileSystem);
	fileSystem->write("test", "ddsgdsgdsgdfddsgdsgdsgdf");
	readNotEmptyFileTest(fileSystem);
	delete  fileSystem;
	std::cout << "read tests passed\n";
}

void renameTest() {
	int error;
	FileSystem *fileSystem = new FileSystem();
	error = fileSystem->initialize(800);
	assert(error == 0);
	error = fileSystem->createFile("test");
	assert(error == 0);
	error = fileSystem->createFile("test1");
	assert(error == 0);

	error = fileSystem->renameFile("testdgdfgggggggggggg", "file");
	assert(error == -2);
	error = fileSystem->renameFile("test", "filefdggggggggggggggggggggggggggf");
	assert(error == -2);
	error = fileSystem->renameFile("test", "test1");
	assert(error == -4);
	error = fileSystem->renameFile("test2", "file");
	assert(error == -3);
	error = fileSystem->renameFile("test", "file");
	assert(error == 0);
	delete  fileSystem;
	std::cout << "rename tests passed\n";
}

void copyTest() {
	std::string str1;
	std::string str2;
	int error;
	FileSystem *fileSystem = new FileSystem();
	error = fileSystem->initialize(800);
	assert(error == 0);
	error = fileSystem->createFile("test");
	assert(error == 0);

	error = fileSystem->copy("testtttttttttttttttttttttttttttt", "file");
	assert(error == -2);
	error = fileSystem->copy("test", "fileeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
	assert(error == -2);
	error = fileSystem->copy("test1", "file");
	assert(error == -3);
	error = fileSystem->copy("test", "file");
	assert(error == 0);
	error = fileSystem->copy("test", "file");
	assert(error == -4);
	error = fileSystem->write("test", "ddsgdsgdsgdfddsgdsgdsgdf");
	assert(error == 0);
	str1 = fileSystem->read("test");
	str2 = fileSystem->read("file");
	assert(str1 != str2);
	error = fileSystem->copy("test", "file1");
	assert(error == 0);
	str1 = fileSystem->read("test");
	str2 = fileSystem->read("file1");
	assert(str1 == str2);
	delete fileSystem;
	std::cout << "copy tests passed\n";
}

void deleteTest() {
	int error;
	FileSystem *fileSystem = new FileSystem();
	error = fileSystem->initialize(800);
	assert(error == 0);
	error = fileSystem->createFile("test");
	assert(error == 0);

	error = fileSystem->deleteFile("dfffffffffffffffffffff");
	assert(error == -2);
	error = fileSystem->deleteFile("test1");
	assert(error == -3);
	error = fileSystem->deleteFile("test");
	assert(error == 0);
	error = fileSystem->deleteFile("test");
	assert(error == -3);
	error = fileSystem->createFile("test");
	assert(error == 0);
	error = fileSystem->deleteFile("test");
	assert(error == 0);
	delete fileSystem;
	std::cout << "delete tests passed\n";
}

void run_all_tests() {
	initTest();
	touchTest();
	writeTest();
	readTest();
	renameTest();
	copyTest();
	deleteTest();
	std::cout << "all tests passed\n";
}

int createFile(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 2) {

		return fileSystem->createFile(wordsVector[1].c_str());
	}
	else {
		return -5;
	}
}

int write(std::vector<std::string> wordsVector) {

	if (wordsVector.size() > 2) {
		std::string text;
		for (int i = 2; i < wordsVector.size(); i++) {
			text += wordsVector[i] + " ";
		}
		return fileSystem->write(wordsVector[1].c_str(), text.c_str());
	}
	else {
		return -5;
	}
}

int read(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 2) {
		std::cout << fileSystem->read(wordsVector[1].c_str()) << "\n";
		return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
	}
	else {
		return -5;
	}
}

int renameFile(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 3) {
		return fileSystem->renameFile(wordsVector[1].c_str(), wordsVector[2].c_str());
	}
	else {
		return -5;
	}
}

int copy(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 3) {
		return fileSystem->copy(wordsVector[1].c_str(), wordsVector[2].c_str());
	}
	else {
		return -5;
	}
}

int removeFile(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 2) {
		return fileSystem->deleteFile(wordsVector[1].c_str());
	}
	else {
		return -5;
	}
}

int showAllFiles(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 1) {
		fileSystem->showAllFiles();
		return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
	}
	else {
		return -5;
	}
}

int dump(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 1) {
		fileSystem->dump();
		return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
	}
	else {
		return -5;
	}
}

int showHelp(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 1) {
		std::cout << "commands:\n"
			<< "TOUCH %filename% - create file \n"
			<< "write %filename% %some text% - write to file\n"
			<< "read %filename% - read file\n"
			<< "rename %old_name% %new_name% - renameFile file\n"
			<< "copy %old_file_name% %new_file_name% - copy file\n"
			<< "delete %filename% - delete file\n"
			<< "ls - show all files\n"
			<< "dump - show file system dump\n"
			<< "cls - clear screen\n"
			<< "close - close file system\n";
		return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
	}
	else {
		return -5;
	}
}

int cls(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 1) {
		system("CLS");
		return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
	}
	else {
		return -5;
	}
}

int closeFileSystem(std::vector<std::string> wordsVector) {

	if (wordsVector.size() == 1) {
		shellState = false;
		return ErrorsCode::SUCCESSFUL_IMPLEMENTATION;
	}
	else {
		return -5;
	}
}

int play(std::vector<std::string> wordsVector) {
	if (wordsVector.size() == 1) {
		std::cout << "Happy New Year!!!" << std::endl;

		Jingle::refrenSolo();
		Jingle::coupleSolo();
		Jingle::refrenSolo();

	}
	else {
		return -5;
	}
}


int main() {
	run_all_tests();
	fileSystem = new FileSystem();
	fileSystem->open("system");

	std::string input;
	std::string word;
	int error = 0;

	functionPtr touchPtr = createFile;
	functionPtr writePtr = write;
	functionPtr readPtr = read;
	functionPtr renamePtr = renameFile;
	functionPtr copyPtr = copy;
	functionPtr removePtr = removeFile;
	functionPtr lsPtr = showAllFiles;
	functionPtr dumpPtr = dump;
	functionPtr helpPtr = showHelp;
	functionPtr clsPtr = cls;
	functionPtr closePtr = closeFileSystem;
	functionPtr jinglePtr = play;

	std::map<std::string, functionPtr> commands = { {"touch", touchPtr},
												   {"write", writePtr},
												   {"read", readPtr},
												   {"renameFile", renamePtr},
												   {"copy", copyPtr},
												   {"removeFile", removePtr},
												   {"ls", lsPtr},
												   {"dump", dumpPtr},
												   {"showHelp", helpPtr},
												   {"cls", clsPtr},
												   {"close", closePtr},
												   {"Jingle!", jinglePtr} };



	while (shellState) {
		std::getline(std::cin, input);
		std::istringstream iss(input, std::istringstream::in);
		std::vector<std::string> wordsVector;

		while (iss >> word) {
			wordsVector.push_back(word);
		}
		std::string commandName = wordsVector[0];
		auto function = commands.find(commandName);
		error = function->second(wordsVector);
		switch (error) {
		case -1:
			std::cout << "lack of memory\n";
			break;
		case -2:
			std::cout << "wrong file name\n";
			break;
		case -3:
			std::cout << "file not found\n";
			break;
		case -4:
			std::cout << "file already exist\n";
			break;
		case -5:
			std::cout << "wrong parameters\n" << '\a';
			break;
		}
	}
	return 0;
}