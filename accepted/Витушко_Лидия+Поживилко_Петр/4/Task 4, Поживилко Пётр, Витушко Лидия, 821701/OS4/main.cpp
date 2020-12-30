#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <stdio.h>
#include <ctime>
using namespace std;


class Block {
public:
    Block(int size) {
        this->size = size;
        data = new char[size];

    }
    char* data;
    int size;
    bool free=true;


};
vector<Block> blocks;
class File {
public:
    File(string text){
        int i=0;
        string str;
        while (i<text.length()+1){
            if(i%8==0&&i!=0||i==text.length()){
                for(int j=0;j<blocks.size();j++){
                    if(blocks[j].free){
                        memcpy(blocks[j].data,str.data(),9);
                        this->add_block(j);
                        str = "";
                        blocks[j].free= false;
                        break;
                    }
                }
            }
            str += text[i];
            i++;


        }
    }
public:
    vector<int> blocks_numbers;
    void add_block(int i) {
        blocks_numbers.push_back(i);
    }
    void print(){
        for(int i=0;i<blocks_numbers.size();i++){
            cout<<blocks[blocks_numbers[i]].data;
        }
    }
    void remove(){
        for(int i=0;i<blocks_numbers.size();i++){
            blocks[blocks_numbers[i]].free = true;
        }

    }

};

void initialize_blocks(int count) {
    srand(time(0));
    for (int i = 0; i < count; i++) {
        Block block = Block(8);
        blocks.push_back(block);
        if (rand() % 3 == 0)
            blocks[i].free = false;
    }
}



int main()
{
    initialize_blocks(100);
    File file = File("You pick the wrong door the leather club is two blocks down");
    file.print();
    file.remove();





}