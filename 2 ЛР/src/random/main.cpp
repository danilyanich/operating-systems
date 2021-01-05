#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>


using namespace std;


void enterArray(int*, int) {

    for (int i = 0; i < size; i++) {
        arr[i] = rand();
    }
}

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        cout << arr[i] << endl;
    }
}

void writeArray(int arr[], int size) {
    ofstream out;
    out.open("array.txt");

    if (out.is_open()) {

        for (int i = 0; i < size; i++) {
            out << arr[i] << endl;;
        }
    }
}


int main(){

    pid_t pid;
    int rv;
    int size;
    int *arr = new int[size];
    cout << "Enter size of the array: ";
    cin >> size;
    enterArray(arr, size);
    
    switch(pid=fork()) {
        case -1:
            perror("fork");
            exit(1);
        case 0:
            printArray(arr, size);
            exit(rv);
        default:
            writeArray(arr, size);
            wait(0);
            
    }
    
    return 0;
}