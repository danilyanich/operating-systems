#include "fileSystem.h"
#include 

int main(){
    FileSystem myFiles;
    std::string command;
    std::string message;
    std::string file_name;
    while(true){
        std::cin>>command;
        if(command == "ls") myFiles.showFiles();
        if(command == "create"){
            std::cin>>file_name;           
            myFiles.createFile(file_name);
        }   
        if(command == "delete"){
             std::cin>>file_name;   
             myFiles.deleteFile(file_name);
            }
        if(command == "read"){
             std::cin>>file_name;   
             myFiles.readFile(file_name);
            }
        if(command == "write"){
             std::cin>>file_name;
             std::cin>>message;   
             myFiles.writeFile(file_name, message);
            }
        if(command == "info"){
            std::cin>>file_name;   
            myFiles.showInfo(file_name);     
        }
        if(command == "quit") return 0;
    }
//	myFiles.createFile("a");
//	myFiles.createFile("b");
//	myFiles.createFile("c");
//	//myFiles.deleteFile("b");
//	myFiles.writeFile("b");
//    myFiles.readFile("b");
//    myFiles.showFiles();
//    myFiles.showInfo("b");
	return 0;
}
