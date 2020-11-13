#include <iostream>
#include  "scr/PhMemory.h"
using namespace std;
int main() {
    cout << "Hello, World!" << std::endl;


    PhMemory phMemory(50);
    cout<< phMemory.getSizeOfMemory()<<endl;
    phMemory.allocateValue(0,'q');
    phMemory.allocateValue(1,'w');
    cout<< phMemory.getValueAt(0)<<endl;
    cout<< phMemory.getAllocatedMemory();
    return 0;
}
