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


class File{

    private:

        string name;
        int * blocks;
        int size;

    public:

        File(string file_name, int size ){
            name = file_name;
        }

        void write(char data[], int size){
            blocks = memory.write(data, size);   
        }

        void delete_file(){
            memory.free(blocks, size);
        }

        void copy(string name_of_copy){
            return;
        }

        char * read(){
            return memory.read(
                blocks,
                size
            );
        }
};


int main(){
    
}