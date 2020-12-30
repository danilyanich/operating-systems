//
// Created by viktor on 09.11.2020.
//

#ifndef FILE_SYSTEM_FILE_SYSTEM_H
#define FILE_SYSTEM_FILE_SYSTEM_H

#include <string>
#include <vector>
#include <ostream>
#include <fstream>
#include <iostream>
#include <ctime>
#include <utility>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

#define SIZE_OF_BLOCK 20
using namespace std;

enum struct code_error {
    OK = 0,
    INCORRECT_ACCESS_RIGHTS = 1,
    STOP = 2,
    NEW_BLOCK = -1
};

enum struct chmode {
    N = 0,
    X = 1,
    W = 2,
    WX = 3,
    R = 4,
    RX = 5,
    RW = 6,
    RWX = 7
};

struct User {
    string chmode = "777";
    string name;
};

struct block {
    string memory;
    unsigned int size_of_block;
};

struct File {
    File(const string &name, const string &extension, const string &date, unsigned int size, int first_block,
         int offset, const string &file_folder_path, const string &attr);

    string name;
    string extension;
    string date;
    unsigned int size;
    int first_block;
    unsigned int offset;
    string file_folder_path;
    string attr;
};

struct Folder {
    Folder(const string &basicString,
           vector<Folder *> vec, vector<File *> vec1,
           const string &basicString1, const string &prev_folder);

    string name;
    vector<Folder *> folders;
    vector<File *> files;
    string attr;
    string prev_folder;
};

class file_system {
private:
    Folder *root;
    Folder *currFolder;
    vector<block> blocks;
public:
    Folder *get_curr_folder() { return this->currFolder; }

    void set_curr_folder(Folder *folder) { this->currFolder = folder; }

    file_system() {
        root = new Folder("root",
                          vector<Folder *>(), vector<File *>(),
                          "777", "");
        currFolder = root;
    }

    static std::string currentDateTime() {
        time_t now = time(nullptr);
        struct tm tstruct{};
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }

    int create_folder(User *user, string &folder_name);

    int delete_folder(User *user, Folder *folder_to_delete);

    int copy_folder(User *user, Folder *folder_to_copy, Folder *destination_folder);

    int move_folder(User *user, Folder *folder_to_move, Folder *destination_folder);

    int create_file(User *user, string &file_name);

    int delete_file(User *user, File *file_to_delete);

    int copy_file(User *user, File *file_to_copy, Folder *destination_folder);

    int move_file(User *user, File *file_to_move, Folder *destination_folder);

    string read_file(User *user, File *file);

    int write_file(User *user, File *file, string &new_data);

    Folder *find_folder(const string &name);

    Folder *find_folder(const string &name, Folder *curr_folder);

    File *find_file(const string &name, Folder *curr_folder);

    int find_block(int start_from, int size_of_memory);

    void write_to_blocks(const string &data, int count_of_blocks, int first_block);

    void print_file_system();

    void print_folder(Folder *folder, int level);

    string print_file_path(File *file) {
        return to_string(*find_folder(file->file_folder_path)) + '/' + file->name + '.' + file->extension;
    }

    string to_string(Folder &folder) {
        string res;
        Folder curr_folder = folder;
        while (!curr_folder.prev_folder.empty()) {
            res.insert(0, '/' + curr_folder.name);
            curr_folder = *find_folder(curr_folder.prev_folder);
        }
        res.insert(0, '/' + curr_folder.name);
        return res;
    };

    void dump(User *user);

    void dump_dir(User* user, Folder* curr_folder);
};

#endif //FILE_SYSTEM_FILE_SYSTEM_H
