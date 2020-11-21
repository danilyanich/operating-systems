// ос1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include "memory.h"

using namespace std;
int main(int argc, char **argv)
{
    int error_code;

    m_init(32,16);

    m_id chunk_1=m_malloc(15, &error_code);
    m_id chunk_2=m_malloc(20, &error_code);
    m_id chunk_3=m_malloc(60, &error_code);

    m_write(chunk_1, "hello world!", 13,&error_code);
    m_write(chunk_2, "operating system", 18,&error_code);
    m_write(chunk_3, "daniil janovich the best professor of iit department", 54,&error_code);

    char buffer1[60];
    char buffer2[60];
    char buffer3[60];

    m_read(chunk_1, buffer1, 13, &error_code,1);
    cout << buffer1 << endl;
    m_read(chunk_1, buffer1, 13, &error_code, 1);
    cout << buffer1 << endl;
    m_read(chunk_2,buffer2, 18, &error_code,2);
    cout << buffer2 << endl;
    m_read(chunk_2, buffer2, 18, &error_code, 2);
    cout << buffer2 << endl;
    m_read(chunk_3, buffer3, 53, &error_code,3);
    cout << buffer3 << endl;
    m_read(chunk_3, buffer3, 53, &error_code, 3);
    cout << buffer3 << endl;
    m_read(chunk_1, buffer1, 13, &error_code, 1);
    cout << buffer1 << endl;
    m_free();
    
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
