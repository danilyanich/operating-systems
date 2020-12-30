#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>


using namespace std;


void enter_array(int*, int);
void print_array(int[], int);
void write_array(int[], int);


int main(){

    pid_t pid;
    int rv;
    int size;
    int *arr = new int[size];
    cout << "Enter size of the array: ";
    cin >> size;
    enter_array(arr, size);
    
    switch(pid=fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            print_array(arr, size);
            exit(rv);
        default:
            write_array(arr, size);
            wait(0);
            
    }
    
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