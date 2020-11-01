#include <iostream>
#include <fstream>
#include <unistd.h>


using namespace std;


void enter_array(int*, int);
void print_array(int[], int);
void write_array(int[], int);


int main(){
    //pid_t fork();

    int size;
    int *arr = new int[size];

    cout << "Enter size of the array: ";
    cin >> size;
    
    enter_array(arr, size);
    print_array(arr, size);
    write_array(arr, size);

    return 0;
}


void enter_array(
    int *arr, 
    int size
){

    for(int i = 0; i < size; i++){
        arr[i] = rand();
    }

}


void print_array(
    int arr[],
    int size
){
    for(int i = 0; i < size; i++){
        cout << arr[i] << endl;
    }
}


void write_array(int arr[], int size){
    ofstream out;
    out.open("array.txt");
    
    if(out.is_open()){
        
        for(int i = 0; i < size; i++){
            out << arr[i] << endl;;
        }
    }
}