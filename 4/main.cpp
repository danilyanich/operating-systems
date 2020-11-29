#include "filesystem.h"
#include <string>


using namespace std;
class Memory{
    private:

        char data[1000];
        int free_blocks[1000];

    public:
    
    Memory(){
        for (int i = 0; i < 1000; i++){
            free_blocks[i] = i;
        }
    }

    char * get_blocks(int numbers[], int size){
        char returnned_blocks[100];
        for (int i = 0; i < size;i ++){
            returnned_blocks[i] = data[numbers[i]];
        }
        return returnned_blocks;
    }

    void write(){
        
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