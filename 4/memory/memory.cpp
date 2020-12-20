#include <iostream>
#include <sstream>

#include "memory.h"

memory* _filesystem = new memory;

void initfs() {
	for (int i = 0; i < 500 / 10; i++) {
		block* new_block = new block;
		new_block->isFilled = false;
		new_block->count = i;
		_filesystem->dump.push_back(new_block);
	}
}

void delete_file(string filename) {
	const size_t dot = filename.find_last_of(".");
	const string extension = filename.substr(dot + 1);
	const string name = filename.substr(0, dot);

	for (auto i = _filesystem->files.begin(); i != _filesystem->files.end(); ++i)
		if ((*i)->name == name && (*i)->extension == extension) {
			for (int block : (*i)->blocks) {
				_filesystem->dump.at(block)->isFilled = false;
				_filesystem->dump.at(block)->data = "";
			}
			i = _filesystem->files.erase(i);

			cout << "Deleted file " << filename << endl;
			return;
		}
}

void ls() {
	for (file* i : _filesystem->files)
		cout << i->name << "." << i->extension << endl;
}

void rename(string old_file, string new_file) {
	const size_t file_dot = old_file.find_last_of(".");
	const string file_extension = old_file.substr(file_dot + 1);
	const string file_name = old_file.substr(0, file_dot);

	const size_t dot = new_file.find_last_of(".");
	const string new_extension = new_file.substr(dot + 1);
	const string new_name = new_file.substr(0, dot);

	for (file* i : _filesystem->files)
		if (i->name == file_name && i->extension == file_extension) {
			i->name = new_name;
			i->extension = new_extension;
			cout << "Successfully renamed " << old_file << " to " << new_file << endl;
			return;
		}

	cout << "File not found: " << old_file << endl;
}

void copy(string filename) {
	const size_t dot = filename.find_last_of(".");
	const string extension = filename.substr(dot + 1);
	const string name = filename.substr(0, dot);

	file* file_to_copy = NULL;
	for (file* i : _filesystem->files)
		if (i->name == name && i->extension == extension) {
			file_to_copy = i;
			break;
		}

	if (!file_to_copy) {
		cout << "Error! File " << filename << " not exists!" << endl;
		return;
	}

	// check if file with _copy exists and add _copy in the end
	string new_name = name + "_copy";
	while (true) {
		bool isExists = checkExist(new_name, extension);
		if (isExists) {
			new_name = new_name + "_copy";
		} else break;
	}

	file* copied_file = touch(new_name + "." + extension);
	write(new_name + "." + extension, file_to_copy->data);
}

bool checkExist(string name, string extension) {
	bool isExists = false;

	for (file* i : _filesystem->files)
		if (i->name == name && i->extension == extension) {
			isExists = true;
			break;
		}

	return isExists;
}

void write(string filename, string data) {
	const size_t dot = filename.find_last_of(".");
	const string extension = filename.substr(dot + 1);
	const string name = filename.substr(0, dot);

	bool isExists = checkExist(name, extension);

	if (!isExists) {
		cout << "Error! File " << filename << " not exists!" << endl;
		return;
	}

	file* file_to_write = NULL;
	for (file* i : _filesystem->files)
		if (i->name == name && i->extension == extension) {
			i->data += data;
			file_to_write = i;
			break;
		}

	if (!file_to_write) return;

	vector<string> arr;
	while (data.length() > 10) {
		string part = data.substr(0, 10);
		arr.push_back(part);
		data = data.substr(10);
	}
	arr.push_back(data);

	for (string i : arr)
		for (block* j : _filesystem->dump) {
			if (!(j->isFilled)) {
				file_to_write->blocks.push_back(j->count);
				j->isFilled = true;
				j->data = i;
				break;
			}
		}

	string indexes = "";
	for (int index : file_to_write->blocks) {
		indexes += to_string(index);
		indexes += " ";
	}

	cout << "successfully wrote to " << filename << "! Block are: " << indexes << endl;
}

void read(string filename) {
	const size_t dot = filename.find_last_of(".");
	const string extension = filename.substr(dot + 1);
	const string name = filename.substr(0, dot);

	for (file* f : _filesystem->files)
		if (f->name == name && f->extension == extension) {
			cout << f->data << endl;
			break;
		}
}

file* touch(string filename) {
	const size_t dot = filename.find_last_of(".");
	const string extension = filename.substr(dot + 1);
	const string name = filename.substr(0, dot);

	bool isExists = checkExist(name, extension);

	if (isExists) {
		cout << "Error! File " << filename << " exists!" << endl;
		return NULL;
	}

	file* new_file = new file;
	new_file->name = name;
	new_file->extension = extension;
	_filesystem->files.push_back(new_file);
	cout << "Successfully created new file " << filename << endl;
	return new_file;
\\return
}