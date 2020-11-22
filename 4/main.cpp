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
            for(int i=0; i<size; i++, last_end_index++){
                data[last_end_index] = writed_data[i];
            }
            return 1;
        }

};

class File{

    private:

        char content[200];
        string name;
        string type;
        int begin;
        int size;

    public:

        const string get_name(){
            return name;
        }

        void set_name(
            const string seted_name
        ){
            name = seted_name;
        }

        const string get_type(){
            return type;
        }

        void set_type(
            const string seted_type
        ){
            type = seted_type;
        }
};

class FileSystem{
    
};

int main(){

}