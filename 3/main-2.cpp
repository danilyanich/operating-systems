#include <iostream>
#include <string>
#include <future>
#include <fstream>
#include <ctime>
#include <chrono>

using namespace std;
const unsigned int MAX_TIME = 3;
const string FILE_BUF = "D:\\Studing\\OS\\Os_3\\My\\ConsoleApplication1\\buffer.txt";

int main(int argc, char* argv[])
{
	if (argc < 2) return 0; 
	int name = stoi(argv[1]);
	string  buffer = "";
	string rezult = to_string((double)(name * (name + 2) * 2 / (name + 3.6)) + ((2.3) * name))
		+ " - rezult ";
	cout << rezult;
	while (true) {
		ifstream check(FILE_BUF);
		if (check.is_open())
		{
			getline(check, buffer);
			if (buffer == argv[1]) break;
			check.close();
		}
		this_thread::sleep_for(0.1ms);
	}

	ofstream out(FILE_BUF, ios::out);
	if (out.is_open()) 
	{
		out << rezult << endl;
		out.close();
	}
	return 0;
}