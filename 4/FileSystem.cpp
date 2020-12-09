#include "FileSystem.h"
FileSystem::FileSystem() {
	File last;
	files.push_back(last);
}

void FileSystem::createFile(std::string name) {
	std::string content;
	std::cout << "������� ���������� ����� : " << std::endl;
	std::cin >> content;
	File rez(name, content);
	for (File &i : files) {
		if (i.getFileName() == "N/A" && i.getSize() >= rez.getSize()) {
			i.setFileName(name);
			i.setContent(content);
			return;
		}
		if (i.getFileName() == "N/A" && i.getSize() == -1) {
			File last;
			i.setFileName(name);
			i.setContent(content);
			files.push_back(last);
			return;
		}
	}
}

void FileSystem::deleteFile(std::string name) {
	for (File &i : files) {
		if (i.getFileName() == name) {
			i.setFileName("N/A");
			i.setContent("N/A");
			return;
		}
	}
	std::cout << "���� � ����� ������ �� ������" << std::endl;
}

std::string FileSystem::readFile(std::string name) {
	std::string rez = {"N/A"};
	for (File &i : files) {
		if (i.getFileName() == name) {
			rez = i.getContent();
			return rez;
		}
	}
	std::cout << "���� � ����� ������ �� ������" << std::endl;
	return rez;
}

void FileSystem::writeFile(std::string name) {
	std::string rez;
	for (File &i : files) {
		if (i.getFileName() == name) {
			std::cout << "������� ����� ���������� �����" << std::endl;
			std::cin >> rez;
			i.setContent(rez);
			return;
		}
	}
	std::cout << "���� � ����� ������ �� ������" << std::endl;
}

File& FileSystem::copy(std::string name) {
	for (File &i : files) {
		if (i.getFileName() == name) {
			return i;
		}
	}
	std::cout << "���� � ����� ������ �� ������" << std::endl;
}