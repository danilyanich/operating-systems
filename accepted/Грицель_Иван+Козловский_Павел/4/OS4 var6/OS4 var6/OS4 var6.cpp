#include <iostream>
#include <string>
#include <vector>

struct memoryBlock {
    memoryBlock* nextBlock = NULL;
    char data;
    bool isEmpty = 1;
};

struct fileTreeNode {
    std::vector<fileTreeNode*> children;
    bool isFolder;
    memoryBlock* content = NULL;
    std::string name;
};

class Memory {
public:
    Memory() {
        memoryBlock* memBlock;
        for (int i = 0; i < 500; i++) {
            memBlock = new memoryBlock;
            memory.push_back(memBlock);
        }
        
    }

    ~Memory() {
        for (int i = 0; i < this->memory.size(); i++) {
            delete this->memory[i];
        }
    }

    memoryBlock* setContent(std::string content) {
        memoryBlock* currentBlock = getEmptyBlock();
        memoryBlock* startBlock = currentBlock;
        for (int i = 0; i < content.length(); i++) {
            currentBlock->data = content[i];
            currentBlock->isEmpty = 0;
            if (!(i == content.length() - 1)) {
                currentBlock->nextBlock = getEmptyBlock();
                currentBlock = currentBlock->nextBlock;
            }
        }
        return startBlock;
    }

    std::string readContent(memoryBlock* block) {
        std::string str = "";
        std::string ch;
        memoryBlock* readFrom = block;
        while (true) {
            str += readFrom->data;
            if (readFrom->nextBlock == NULL) {
                break;
            }
            else {
                readFrom = readFrom->nextBlock;
            }
        }
        return str;
    }

    void deleteContent(memoryBlock* block) {
        memoryBlock* currentBlock = block;
        if (currentBlock->nextBlock != NULL) {
            deleteContent(currentBlock->nextBlock);
        }
        currentBlock->isEmpty = 1;
        currentBlock->nextBlock = NULL;
    }
private:
    std::vector<memoryBlock*> memory;

    memoryBlock* getEmptyBlock() {
        for (int i = 0; i < memory.size(); i++) {
            if (memory[i]->isEmpty) {
                memory[i]->isEmpty = 0;
                return memory[i];
            }
        }
        return NULL;
    }
    
};

class FileSystem {
public:
    std::string path = "root/";

    FileSystem() {
        mem = new Memory(); 
        root = new fileTreeNode;
        root->isFolder = true;
        root->name = "root";
    }

    std::string createFile(std::string name) {
        if (name == "..") {
            return("ERROR: Inappropriate name");
        }
        fileTreeNode* node = getNodeByPath(parseWay(path));
        if (!node->isFolder) {
            return("\"" + path + "\" is not a directory");
        }
        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i]->name == name) {
                return("This file already exist");
            }
        }
        std::vector<fileTreeNode*> newV;
        fileTreeNode* newNode = new fileTreeNode{ newV, 0, NULL, name };
        node->children.push_back(newNode);
        return ("\"" + name + "\" file created");
    }

    std::string createFolder(std::string name) {
        if (name == "..") {
            return("ERROR: Inappropriate name");
        }
        fileTreeNode* node = getNodeByPath(parseWay(path));
        if (!node->isFolder) {
            return("\"" + path + "\" is not a directory");
        }
        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i]->name == name) {
                return("This folder already exist");
            }
        }
        std::vector<fileTreeNode*> newV;
        fileTreeNode* newNode = new fileTreeNode{ newV, 1, NULL, name };
        node->children.push_back(newNode);
        return ("\"" + name + "\" folder created");
    }

    std::string openFile(std::string name) {
        fileTreeNode* node = getNodeByPath(parseWay(path + name + "/"));
        if (node->isFolder) {
            return "ERROR: \"" + name + "\" is not a file";
        }
        std::cout << name << " file opened, type \"/end\" to finish" << std::endl << std::endl;
        std::string cont;
        if (node->content != NULL) {
            cont = mem->readContent(node->content);
            std::cout << cont;
        }
        std::string add;
        while (true) {
            std::cin >> add;
            if (add == "/end") {
                break;
            }
            cont += add;
            cont += " ";
        }
        node->content = mem->setContent(cont);
        return "";
    }

    std::string readFile(std::string path) {
        fileTreeNode* node = getNodeByPath(parseWay(path));
        if (node->isFolder) {
            return("ERROR: Can't read from folder");
        }
        std::string str = mem->readContent(node->content);
        return str;
    }

    void deleteElement(std::string el) {
        fileTreeNode* node = getNodeByPath(parseWay(path + el + "/"));
        fileTreeNode* node1 = getNodeByPath(parseWay(path));
        std::vector<fileTreeNode*> v;
        for (int i = 0; i < node1->children.size(); i++) {
            if (node1->children[i] != node) {
                v.push_back(node1->children[i]);
            }
        }
        node1->children.clear();
        for (int i = 0; i < v.size(); i++) {
            node1->children.push_back(v[i]);
        }
        if (node->isFolder) {
            deleteFolder(node);
        }
        else {
            deleteFile(node);
        }
    }

    std::string cd(std::string name) {
        fileTreeNode* node = getNodeByPath(parseWay(path));
        if (name == "..") {
            std::vector<std::string> v = parseWay(path);
            if (v.size() == 1) {
                return "";
            }
            path = "";
            for (int i = 0; i < v.size() - 1; i++) {
                path += v[i];
                path += "/";
            }
            return ("");
        }
        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i]->name == name) {
                if (!node->children[i]->isFolder) {
                    return("ERROR: \"" + name + "\" is not a directory");
                }
                path += name;
                path += "/";
                return ("Switched to \"" + name + "\" directory");
            }
        }
        return "ERROR: No such directory";
    }

    void vis() {
        std::cout << path << std::endl;
        fileTreeNode* node = getNodeByPath(parseWay(path));
        if (node->children.size() == 0) {
            std::cout << std::endl << "This directory is empty" << std::endl;
            return;
        }
        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i]->isFolder) {
                std::cout << "D: ";
            }
            else {
                std::cout << "F: ";
            }
            std::cout << node->children[i]->name << std::endl;
        }
    }

    void cut(std::string name) {
        //copyNode = getNodeByPath(parseWay(path + name + "/"));
        fileTreeNode* node = getNodeByPath(parseWay(path + name + "/"));
        copyNode = new fileTreeNode{ node->children, node->isFolder, node->content, node->name };
        fileTreeNode* node1 = getNodeByPath(parseWay(path));
        std::vector<fileTreeNode*> v;
        for (int i = 0; i < node1->children.size(); i++) {
            if (node1->children[i]->name != name) {
                v.push_back(node1->children[i]);
            }
        }
        node1->children = v;
    }

    void copy(std::string name) {
        fileTreeNode* node = getNodeByPath(parseWay(path + name + "/"));
        copyNode = new fileTreeNode{ node->children, node->isFolder, node->content, node->name };
    }
    
    std::string paste() {
        if (copyNode == NULL) {
            return "";
        }
        fileTreeNode* node = getNodeByPath(parseWay(path));
        for (int i = 0; i < node->children.size(); i++) {
            if (node->children[i] == copyNode) {
                return "ERROR: Can't copy";
            }
        }
        node->children.push_back(copyNode);
        return "";
    }

private:
    Memory* mem;
    fileTreeNode* root;
    fileTreeNode* copyNode = NULL;

    void deleteFile(fileTreeNode* node) {
        if (node->content != NULL) {
            mem->deleteContent(node->content);
        }
        delete node;
    }

    void deleteFolder(fileTreeNode* node) {
        if (node->children.size() != 0) {
            for (int i = 0; i < node->children.size(); i++) {
                if (node->children[i]->isFolder) {
                    deleteFolder(node->children[i]);
                }
                else {
                    deleteFile(node->children[i]);
                }
            }
        }
        node->children.clear();
        delete node;
    }

    std::vector<std::string> parseWay(std::string path) {
        int pos;
        std::string tPath = path;
        std::vector <std::string> s1;

        while (tPath.find("/") != -1) {
            pos = tPath.find("/");
            if (pos == 0) {
                break;
            }

            s1.push_back(tPath.substr(0, pos));
            tPath.erase(0, pos + 1);
        }

        return s1;
    }

    fileTreeNode* getNodeByPath(std::vector<std::string> path) {
        fileTreeNode* node = root;
        if (path.size() == 1) {
            return node;
        }
        for (int i = 1; i < path.size(); i++) {
            for (int j = 0; j < node->children.size(); j++) {
                if (node->children[j]->name == path[i]) {
                    node = node->children[j];
                    break;
                }
            }
        }
        return node;
    }
};

int main()
{
    FileSystem fs = FileSystem();
    std::string command;
    std::string argument;
    while (true) {
        fs.vis();
        std::cout << std::endl << std::endl;
        std::cin >> command;
        if (command == "exit" || command == "quit") {
            return 0;
        }
        if (command == "md") {
            std::cin >> argument;
            std::cout << fs.createFolder(argument) << std::endl;
        }
        if (command == "cd") {
            std::cin >> argument;
            std::cout << fs.cd(argument) << std::endl;
        }
        if (command == "del") {
            std::cin >> argument;
            fs.deleteElement(argument);
        }
        if (command == "cf") {
            std::cin >> argument;
            std::cout << fs.createFile(argument) << std::endl;
        }
        if (command == "open") {
            std::cin >> argument;
            std::cout << fs.openFile(argument) << std::endl;
        }
        if (command == "cut") {
            std::cin >> argument;
            fs.cut(argument);
        }
        if (command == "copy") {
            std::cin >> argument;
            fs.copy(argument);
        }
        if (command == "paste") {
            std::cout << fs.paste();
        }
        std::cout << std::endl << std::endl;
    }
}