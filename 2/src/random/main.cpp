#include <iostream>

using namespace std;


void enter_array(int*, int);
void print_array(int[], int);


int main(){
    int size;
    int *arr = new int[size];

    cout << "Enter size of the array: ";
    cin >> size;
    
    enter_array(arr, size);
    print_array(arr, size);

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