#include "filesystem.h"
#include <string>

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

    char * get_blocks(int numbers[], int size){
        char returnned_blocks[100];
        for (int i = 0; i < size;i ++){
            returnned_blocks[i] = data[numbers[i]];
        }
        return returnned_blocks;
    }

    int * write(char write_data[], int size){
        int numbers[100];
        for (int i = 0; i < size; i++){
            data[free_blocks[i]] = write_data[i];
            numbers[i] = free_blocks[i];
        }
        _take_memory(size);
        return numbers;
    }

    void delete_file(int numbers[], int size){
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

class File{

    private:

        char content[200];
        string name;
        int begin;
        int size;

    public:
        File(
            char conent[],
            string name,
            int size
        ){
            
        }

};

class FileSystem{
    
};

int main(){

}