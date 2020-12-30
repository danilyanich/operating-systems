#include <iostream>
#include "string"
#include "file_system.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    file_system fileSystem;
    User user{ "777", "admin" };
    while (true) {
        fileSystem.print_file_system();
        string answer;
        getline(cin, answer);
        string command = answer.substr(0, answer.find_first_of(' '));
        if (answer == "quit" || answer == "exit") {
            exit(0);
        }
        if (command == "mkdir") {
            string folderName = answer.substr(answer.find_first_of(' ') + 1);
            fileSystem.create_folder(&user, folderName);
            continue;
        }
        if (command == "cd") {
            string folderName = answer.substr(answer.find_first_of(' ') + 1);
            if (folderName == "..") {
                fileSystem.set_curr_folder(fileSystem.find_folder(fileSystem.get_curr_folder()->prev_folder));
            }
            else {
                if (fileSystem.find_folder(folderName, fileSystem.get_curr_folder()) == nullptr) {
                    cout << "unknown directory\n";
                    continue;
                }
                fileSystem.set_curr_folder(fileSystem.find_folder(folderName, fileSystem.get_curr_folder()));
            }
            continue;
        }
        if (command == "cpdir") {
            unsigned int len = answer.find_last_of(' ') - answer.find_first_of(' ') - 1;
            string folderNameToCopy = answer.substr(answer.find_first_of(' ') + 1, len);
            string destinationFolderName = answer.substr(answer.find_last_of(' ') + 1);
            fileSystem.copy_folder(&user,
                fileSystem.find_folder(folderNameToCopy, fileSystem.get_curr_folder()),
                fileSystem.find_folder(destinationFolderName, fileSystem.get_curr_folder())
            );
            continue;
        }
        if (command == "mvdir") {
            unsigned int len = answer.find_last_of(' ') - answer.find_first_of(' ') - 1;
            string folderNameToMove = answer.substr(answer.find_first_of(' ') + 1, len);
            string destinationFolderName = answer.substr(answer.find_last_of(' ') + 1);
            fileSystem.move_folder(&user,
                fileSystem.find_folder(folderNameToMove, fileSystem.get_curr_folder()),
                fileSystem.find_folder(destinationFolderName, fileSystem.get_curr_folder())
            );
            continue;
        }
        if (command == "rmdir") {
            string folderName = answer.substr(answer.find_first_of(' ') + 1);
            fileSystem.delete_folder(&user, fileSystem.find_folder(folderName, fileSystem.get_curr_folder()));
            continue;
        }
        if (command == "touch") {
            string fileName = answer.substr(answer.find_first_of(' ') + 1);
            auto pos = fileName.find('.');
            if (pos == string::npos) {
                cout << "Enter file name with extension" << endl;
                continue;
            }
            fileSystem.create_file(&user, fileName);
            continue;
        }
        if (command == "cp") {
            unsigned int len = answer.find_last_of(' ') - answer.find_first_of(' ') - 1;
            string fileNameToCopy = answer.substr(answer.find_first_of(' ') + 1, len);
            string destinationFolderName = answer.substr(answer.find_last_of(' ') + 1);
            fileSystem.copy_file(&user,
                fileSystem.find_file(fileNameToCopy, fileSystem.get_curr_folder()),
                fileSystem.find_folder(destinationFolderName, fileSystem.get_curr_folder())
            );
            continue;
        }
        if (command == "mv") {
            unsigned int len = answer.find_last_of(' ') - answer.find_first_of(' ') - 1;
            string fileNameToMove = answer.substr(answer.find_first_of(' ') + 1, len);
            string destinationFolderName = answer.substr(answer.find_last_of(' ') + 1);
            fileSystem.move_file(&user,
                fileSystem.find_file(fileNameToMove, fileSystem.get_curr_folder()),
                fileSystem.find_folder(destinationFolderName, fileSystem.get_curr_folder())
            );
            continue;
        }
        if (command == "rm") {
            string fileName = answer.substr(answer.find_first_of(' ') + 1);
            fileSystem.delete_file(&user, fileSystem.find_file(fileName, fileSystem.get_curr_folder()));
            continue;
        }
        if (command == "cat") {
            string fileName = answer.substr(answer.find_first_of(' ') + 1);
            cout << fileSystem.read_file(&user, fileSystem.find_file(fileName, fileSystem.get_curr_folder())) << endl;
            continue;
        }
        if (command == "echo") {
            //some text>>file.txt
            unsigned int textLen = answer.find_first_of(">>") - answer.find_first_of(' ') - 1;
            string text = answer.substr(answer.find_first_of(' ') + 1, textLen);
            string file = answer.substr(answer.find_first_of(">>") + 2);
            auto pos = file.find('.');
            if (pos == string::npos) {
                cout << "Enter file name with extension" << endl;
                continue;
            }
            fileSystem.write_file(&user, fileSystem.find_file(file, fileSystem.get_curr_folder()), text);
            continue;
        }
        if (command == "dump") {
            fileSystem.dump(&user);
            continue;
        }
        else {
            cout << "Unknown command\n";
            continue;
        }
    }
}
