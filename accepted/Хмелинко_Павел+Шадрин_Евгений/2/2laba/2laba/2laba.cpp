// OC_2_laba.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <windows.h>
#include <thread>
#include "windows.h"
#include <string>

using namespace std;

HANDLE mut;
LPCWSTR str = L"HELLO";
DWORD result;
int count(0);


void imitation_of_parikmaher() {

    result = WaitForSingleObject(mut, INFINITE);

    if (result == WAIT_OBJECT_0) {

        cout << "parikmaher rabotaet" << endl;

        this_thread::sleep_for(chrono::milliseconds(1000));

        ::count--;

        cout << "parikmaher shget " << endl;
        if (::count > 0) {
            ReleaseMutex(mut);
        } else {
            cout << "parikmaher spit" << endl;
        }
    } else {
        cout << "vi budite parikmahera" << endl;
        ReleaseMutex(mut);
        imitation_of_parikmaher();
    }
}

void imitation_of_people() {
    int const lenght = 3;
    int iteration = 0;
    thread count_of_ppl[lenght];
    cout << "ustanovleno max ppl:" << lenght << endl;

    while (true) {

        this_thread::sleep_for(chrono::milliseconds(2000));
        cout << "zahodit chelovek" << endl;

        if (::count + 1 > lenght) {
            cout << "zaiditi posge" << endl;
        } else {

            if (iteration >= lenght) {
                for (int i = iteration % 3; i < lenght;) {
                    if (count_of_ppl[i].joinable()) {
                        count_of_ppl[i].join();
                        break;
                    }
                }
            }
            ::count++;

            for (int i = 0; i < lenght; i++) {
                if (!count_of_ppl[i].joinable()) {
                    count_of_ppl[i] = thread(imitation_of_parikmaher);
                    break;
                }
            }

        }

        iteration++;
    }

}


int main() {
    mut = CreateMutex(NULL, FALSE, str);

    imitation_of_people();


    CloseHandle(mut);
}
