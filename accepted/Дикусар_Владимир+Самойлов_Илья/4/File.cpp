#include "File.h"
File::File() {
	fileName = "N/A";
	content = "N/A";
	size = -1;
}

File::File(std::string name, std::string Content) {
	fileName = name;
	content = Content;
	size = content.size();
}

void File::setFileName(std::string name) {
	fileName = name;
}

std::string File::getFileName() {
	return fileName;
}

void File::setContent(std::string Content) {
	content = Content;
	if (size == -1) {
		size = content.size();
	}
}

std::string File::getContent() {
	return content;
}

int File::getSize() {
	return size;
}