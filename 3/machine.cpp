#include <iostream>
#include <vector>
#include <string>
#include <future>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <process.h>
using namespace std;
using std::string;
const unsigned int MACHINES_COUNT = 10; 
const string FILE_OPEN = "D:\\Studing\\OS\\Os_3\\My\\ConsoleApplication1\\result.txt"; 
const string FILE_BUF = "D:\\Studing\\OS\\Os_3\\My\\ConsoleApplication1\\buffer.txt";
string MACHINE = "D:\\Studing\\OS\\Os_3\\My\\ConsoleApplication1\\machine.exe"; 

vector<PROCESS_INFORMATION> proc_infs; 
vector<string> proc_names; 

void proc_inf_init();
int zeroMachine()
{
	ofstream check(FILE_OPEN, ios::out);
	if (check.is_open()) check.close();
	string  buffer = "";
	for (int i = 0; i < MACHINES_COUNT; i++) {

		ofstream check(FILE_BUF, ios::out);
		if (check.is_open()) { 
			cout << "------" << proc_names[i] << endl;
			check << proc_names[i] << endl; 
			check.close();
		}

		WaitForSingleObject(proc_infs[i].hProcess,	INFINITE);
		this_thread::sleep_for(0.05s);
		CloseHandle(proc_infs[i].hProcess);
		CloseHandle(proc_infs[i].hThread);

		ifstream buf(FILE_BUF);
		if (buf.is_open()) {
			getline(buf, buffer);
			buf.close();
		}
		ofstream out(FILE_OPEN, ios::app); 
		if (out.is_open()) {
			out << buffer << endl; 
			out.close();
		}
	}
	return 0;
};

int workMachine(int name, PROCESS_INFORMATION& pi)
{
	STARTUPINFOA si = { sizeof(si) }; // èíôà 
	string strs = MACHINE;
	strs += " ";	
	strs += to_string(name);
	proc_names.push_back(to_string(name));	
	cout << strs << endl; 
	if (CreateProcessA(NULL, const_cast<char*>(strs.c_str()), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi) == TRUE)
	{ 
		cout << "handle " << pi.hProcess << endl;
	}
	return 0;
}
// main
int main(int argc, char** arg)
{
	proc_inf_init();
	for (int i = 0; i < MACHINES_COUNT; i++) // ñîçäàíèå n ìàøèí
		workMachine(i, proc_infs[i]);
	future<int> zero_m = async(std::launch::async, zeroMachine);
	zero_m.get();
	return 0;
}

void proc_inf_init() {
	for (int i = 0; i < MACHINES_COUNT; i++) {
		PROCESS_INFORMATION pi;
		proc_infs.push_back(pi);
	}
}