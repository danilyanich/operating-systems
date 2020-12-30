#include <iostream>
#include <ctime>
#include <string>
#include <vector>
#include <fstream>
#pragma warning(disable : 4996)

using namespace std;

struct memory
{
	bool used = false;
};

struct refs
{
	int ref = -1;
};

struct file
{
	int size;
	char *data;
	string name;
	string container;
	int ref;
};

void init(int memory_size);

void run(vector<memory> &mem, vector<refs> &ref);

void Help();

void View_all(vector<file> files);

void Copy(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName);

void Move(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName);

void Create(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName);

void Read(vector<file> files, string fileName);

void Write(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName);

void mkDump(vector<memory> &mem, vector<refs> &ref, vector<file> &files);

void ldDump(vector<memory> &mem, vector<refs> &ref, vector<file> &files);

void Delete(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName);

void View_memory(vector<memory> &mem, vector<refs> &ref);

int main()
{
	init(100);
	return 0;
}

void init(int memory_size)
{
	vector<memory> mem(memory_size);
	vector<refs> ref(memory_size);
	run(mem, ref);
}

void run(vector<memory> &mem, vector<refs> &ref)
{

	int command;
	string fileName;
	vector<file> files;
	while (1)
	{
		int memory_size = 0;
		for (int i = 0; i < mem.size(); i++)
		{
			if (mem[i].used == false)
				memory_size++;
		}
		cout.clear();
		cout << endl;
		cout << "Total memory: " + to_string(memory_size) << endl;
		Help();
		cin >> command;
		switch (command)
		{
		case 1:
			View_all(files);
			break;
		case 2:
			cout << "Please, enter a file name: ";
			cin >> fileName;
			Create(mem, ref, files, fileName);
			break;
		case 3:
			cout << "Please, enter a file name: ";
			cin >> fileName;
			Copy(mem, ref, files, fileName);
			break;
		case 4:
			cout << "Please, enter a file name: ";
			cin >> fileName;
			Move(mem, ref, files, fileName);
			break;
		case 5:
			cout << "Please, enter a file name: ";
			cin >> fileName;
			Read(files, fileName);
			break;
		case 6:
			cout << "Please, enter a file name: ";
			cin >> fileName;
			Write(mem, ref, files, fileName);
			break;
		case 7:
			mkDump(mem, ref, files);
			break;
		case 8:
			ldDump(mem, ref, files);
			break;
		case 9:
			cout << "Please, enter a file name: ";
			cin >> fileName;
			Delete(mem, ref, files, fileName);
			break;
		case 10:
			return;
			break;
		case 11:
			View_memory(mem, ref);
		}
	}
}

void Help()
{
	cout << "1 - view all, 2 - create file, 3 - copy, 4 - move" << endl;
	cout << "5 - read, 6 - write, 7 -make dump, 8 - load dump, 9 - delete, 10 - exit" << endl;
}

void View_all(vector<file> files)
{
	cout << "name|size|ref" << endl;
	for (int i = 0; i < files.size(); i++)
	{
		cout << files[i].name + "    " + to_string(files[i].size) + "  " + to_string(files[i].ref) << endl;
	}
	return;
}

void Copy(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName)
{
	int addToFileName = 0;
	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].name == fileName)
		{
			for (int j = 0; j < files.size(); j++)
			{
				if (files[j].name == fileName + to_string(addToFileName))
				{
					addToFileName++;
					j = -1;
				}
				if (j == files.size() - 1)
				{
					vector<int> freeBlocks;
					for (int q = 0; q < mem.size(); q++)
					{
						if (mem[q].used == false)
						{
							freeBlocks.push_back(q);
						}
					}
					if (freeBlocks.size() >= files[i].size)
					{
						files.push_back(files[i]);
						files[files.size() - 1].name = fileName + to_string(addToFileName);
						files[files.size() - 1].ref = freeBlocks[0];
						for (int q = 0; q < files[files.size() - 1].size; q++)
						{
							mem[freeBlocks[0]].used = true;
							if (q != files[files.size() - 1].size - 1)
								ref[freeBlocks[0]].ref = freeBlocks[1];
							else
								ref[freeBlocks[0]].ref = -2;
							freeBlocks.erase(freeBlocks.begin());
						}
						return;
					}
					else
						cout << "Sorry, you don't have enough memory";
				}
			}
		}
	}
}

void Move(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName)
{
	vector<int> freeBlocks;
	file fl;
	for (int q = 0; q < mem.size(); q++)
	{
		if (mem[q].used == false)
		{
			freeBlocks.push_back(q);
		}
	}
	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].name == fileName)
			if (freeBlocks.size() >= files[i].size)
			{
				fl = files[i];
				Delete(mem, ref, files, fileName);
				files.push_back(fl);
				files[files.size() - 1].ref = freeBlocks[0];
				for (int q = 0; q < files[files.size() - 1].size; q++)
				{
					mem[freeBlocks[0]].used = true;
					if (q != files[files.size() - 1].size - 1)
						ref[freeBlocks[0]].ref = freeBlocks[1];
					else
						ref[freeBlocks[0]].ref = -2;
					freeBlocks.erase(freeBlocks.begin());
				}
				return;
			}
			else
			{
				cout << "Sorry, you don't have enough memory";
				return;
			}
	}
}

void Create(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName)
{
	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].name == fileName)
		{
			cout << "Sorry, file with such name is already exist" << endl;
			return;
		}
	}
	file newFile;
	newFile.container = "";
	newFile.name = fileName;
	newFile.size = 1;
	time_t now = time(0);
	char *data = ctime(&now);
	newFile.data = data;
	for (int i = 0; i < mem.size(); i++)
	{
		if (mem[i].used == false)
		{
			mem[i].used = true;
			ref[i].ref = -2;
			newFile.ref = i;
			files.push_back(newFile);
			break;
		}
		if (i == mem.size() - 1)
		{
			cout << "Sorry, you don't have enough memory" << endl;
		}
	}
	return;
}

void Read(vector<file> files, string fileName)
{
	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].name == fileName)
		{
			cout << files[i].container << endl;
			return;
		}
	}
}

void Write(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName)
{
	string container;
	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].name == fileName)
		{
			cout << "Input text: ";
			cin >> container;
			vector<int> freeBlocks;
			for (int q = 0; q < mem.size(); q++)
			{
				if (mem[q].used == false)
				{
					freeBlocks.push_back(q);
				}
			}
			int re = files[i].ref;
			int tmp;
			while (re != -2)
			{
				mem[re].used = false;
				tmp = re;
				re = ref[re].ref;
				ref[tmp].ref = -1;
			}
			mem[files[i].ref].used = true;
			ref[files[i].ref].ref = -2;
			int size = container.size();
			if (freeBlocks.size() >= size)
			{
				files[i].container = container;
				int re = files[i].ref;
				while (1)
				{
					if (re < 0 || ref[re].ref < -1)
						break;
					re = ref[re].ref;
				}
				files[i].size = size + 1;
				while (size > 0)
				{
					ref[re].ref = freeBlocks[0];
					mem[re].used = true;
					size--;
					re = freeBlocks[0];
					freeBlocks.erase(freeBlocks.begin());
				}
				mem[re].used = true;
				ref[re].ref = -2;
			}
			else
			{
				cout << "Sorry, you don't have enough mempry" << endl;
				break;
			}
		}
	}
}

void mkDump(vector<memory> &mem, vector<refs> &ref, vector<file> &files)
{
	ofstream out;
	out.open("M:\\Dump.txt", ios::trunc);
	if (out.is_open())
	{
		for (int i = 0; i < mem.size(); i++)
		{
			out << ref[i].ref;
			out << "\n";
			out << mem[i].used;
			out << "\n";
		}
		out << "\n-1-1-1-1-1-1-1-1-1-1\n";
		for (int i = 0; i < files.size(); i++)
		{
			out << files[i].container;
			out << "\n";
			out << files[i].data;
			out << files[i].name;
			out << "\n";
			out << files[i].ref;
			out << "\n";
			out << files[i].size;
			out << "\n";
		}
		out << "Done";
	}
	else
		cout << "ERROR" << endl;
	out.close();
}

void ldDump(vector<memory> &mem, vector<refs> &ref, vector<file> &files)
{
	mem.clear();
	files.clear();
	ifstream in;
	in.open("M:\\Dump.txt");
	if (in.is_open())
	{
		string tmString;
		memory memr;
		refs re;
		file fl;
		while (!in.eof())
		{
			while (1)
			{
				getline(in, tmString);
				if (tmString == "-1-1-1-1-1-1-1-1-1-1" || in.eof())
					break;
				if (tmString == "")
					continue;
				re.ref = stoi(tmString);
				getline(in, tmString);
				memr.used = stoi(tmString);
				mem.push_back(memr);
				ref.push_back(re);
			}
			while (1)
			{
				getline(in, tmString);
				fl.container = tmString;
				getline(in, tmString);
				if (tmString == "Done" || in.eof())
				{
					break;
				}

				fl.data = _strdup(tmString.c_str());
				getline(in, tmString);
				fl.name = tmString;
				getline(in, tmString);
				fl.ref = stoi(tmString);
				getline(in, tmString);
				fl.size = stoi(tmString);
				files.push_back(fl);
			}
		}
	}
	else
		cout << "ERROR" << endl;
	in.close();
}

void Delete(vector<memory> &mem, vector<refs> &ref, vector<file> &files, string fileName)
{
	for (int i = 0; i < files.size(); i++)
	{
		if (files[i].name == fileName)
		{
			int re = files[i].ref;
			int tmp;
			files.erase(files.begin() + i);
			while (re != -2)
			{
				mem[re].used = false;
				tmp = re;
				re = ref[re].ref;
				ref[tmp].ref = -1;
			}
			return;
		}
	}
}

void View_memory(vector<memory> &mem, vector<refs> &ref)
{
	for (int i = 0; i < ref.size(); i++)
	{
		cout << ref[i].ref << endl;
	}
	cout << endl;
	for (int i = 0; i < mem.size(); i++)
	{
		cout << mem[i].used << endl;
	}
}