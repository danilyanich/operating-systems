#include "filesystem.h"
#include <string>

using namespace std;

class Memory{
    private:

        char data[1000];
        int last_end_index = 0;

    public:

        int write(
            char writed_data[],
            int size
        ){
            int begin = last_end_index;
            for(int i=0; i<size; i++, last_end_index++){
                data[last_end_index] = writed_data[i];
            }
            return begin;
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