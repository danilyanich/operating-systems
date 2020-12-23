// OS-4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iterator>
#include <iomanip>


#define MEMORY_SIZE 512
using namespace std;

struct file {
	string name;
	string type;
	vector<int> indexes;
};

struct catalog {
	vector<file> files;
};

vector<string> memory(MEMORY_SIZE);
catalog root;

void dir() {
	for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
		cout << it->name << '.' << it->type << endl;
	}
}

bool checkForFileName(string fileName, string fileType) {
	for (vector<file>::iterator it = root.files.begin(); it < root.files.end(); it++) {
		if (it->name == fileName && it->type == fileType) {
			return true;
		}
	}
	return false;
}
void createFile(string fileName, string fileType) {
	if (!checkForFileName(fileName, fileType)) {
		file newFile;
		newFile.name = fileName;
		newFile.type = fileType;
		root.files.push_back(newFile);
	}
}

int main()
{
    std::cout << "Hello World!\n";
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
