#include <string>
#include <iostream>
using namespace std;

class Memory{

        char data[1000];
        int free_blocks[1000];
        int free_size;

public:
    
    Memory(){
        for (int i = 0; i < 1000; i++){
            free_blocks[i] = i;
        }
        free_size = 1000;
    }

    char * read(int numbers[], int size){
        static char returnned_blocks[100];
        for (int i = 0; i < size;i ++){
            returnned_blocks[i] = data[numbers[i]];
        }
        return returnned_blocks;
    }

    int * write(char write_data[], int size){
        static int numbers[100];
        for (int i = 0; i < size; i++){
            data[free_blocks[i]] = write_data[i];
            numbers[i] = free_blocks[i];
        }
        _take_memory(size);
        return numbers;
    }

    void free(int numbers[], int size){
        for (int i = i; i < size; i++){
            free_blocks[free_size+i] = numbers[i];
        }
        free_size += size;
    }
        
private: 

    void _take_memory(int size){
        free_size -= size;
        for (int i = 0; i < free_size; i++ ){
            free_blocks[i] = free_blocks[i+size];
        }
    }

}memory;


class File {

        string name;
        int * blocks;
        int size;

    public:

        File(string file_name=""){
            name = file_name;
        }

        void write(char data[], int size){
            this->size = size;
            blocks = memory.write(data, size);
        }

        void delete_file(){
            memory.free(blocks, size);
        }

        void copy(string name_of_copy){
            if (name_of_copy == name){
                cout << "Wrong name" << endl;
            }
            else{
                File copy_of_file = File(name_of_copy);
                copy_of_file.write(
                    this->read(),
                    this->size
                );
            }
        }

        char * read(){
            return memory.read(
                blocks,
                size
            );
        }

        int get_size(){
            return size;
        }

        string get_name(){
            return name;
        }
};

class FileSystem{

    File files[100];
    int number_of_files;

public:

    FileSystem(){
        number_of_files = 0;
    }

    void start(){
        bool loop = true;
        while (loop){
            int number;
            interface();
            cout << "Enter the number: ";
            cin >> number;
            
            switch (number)
            {
            case 1:
                show_files();
                break;
            case 2:{
                    string name;
                    cout << "Enter a name of a file: ";
                    cin >> name;
                    delete_file(name);
                    break;
                }
            case 3:{
                    string name;
                    cout << "Enter a name of a file: ";
                    cin >> name;
                    add_file(name);
                    break;
                }
            case 4:
                break;
            case 5:
                break;
            case 0:
                loop = false;
            }
            cout << "\n\n";
        }
    }

    void interface(){
        cout << "1 - Show files" << endl;
        cout << "2 - Delete a file" << endl;
        cout << "3 - Add a file" << endl;
        cout << "4 - Write in a file" << endl;
        cout << "5 - Copy a file" << endl;
        cout << "0 - Exit" << endl;
    }

    void show_files(){
        for(int i=0; i < number_of_files; i++){
            cout << files[i].get_name() << endl;
        }
    }
    
    void add_file(string name){
        files[number_of_files] = File(name);
        number_of_files++;
    }

    void show_content_of_file(string name){
        int index = _find_file(name);
        char * content = files[index].read();
        for(int i=0; i < files[index].get_size(); i++){
            cout << content[i];
        }
        cout << endl;
    }

    void delete_file(string name){
        int index = _find_file(name);
        for (int i=index; i < number_of_files; i++){
            files[i] = files[i+1];
        }
        number_of_files --;
    }

private:

    int _find_file(string name){
        for (int i = 0; i < number_of_files; i++){
            if (files[i].get_name() == name){
                return i;
            }
        }
    }

}filesystem;


int main(){
    filesystem.start();
}
