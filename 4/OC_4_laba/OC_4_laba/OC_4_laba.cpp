// OC_4_laba.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct node {
    int begin_of_file; //адресс яйчейки с которой считывается файл
    bool is_catalogue; //булеан, является ли текущий узел каталогом
    string name;

    //указатель на след. блок данных(файл или каталог) на текущем уровне
    node *files_and_catalogues_on_curr_lvl;
    //указатель на след. блок данных(файл или каталог) на следующем уровне
    node *files_and_catalogues_on_next_lvl;
};

class File_hierarchy {
public:
    File_hierarchy() {
        root = new node;

        root->is_catalogue = true;
        root->name = "root";
        root->files_and_catalogues_on_curr_lvl = NULL;
        root->files_and_catalogues_on_next_lvl = NULL;
        root->begin_of_file = -1;
    }

    ~File_hierarchy() {}

    void create_file(int begin_of_file, string way, string name, bool is_catalogue) {
        vector <string> s1 = parse_way(way);
        if (s1.size() == 0) {
            return;
        }

        node *tmp;
        tmp = find_data_object(s1);
        if (tmp == NULL) {
            return;
        }
        if (tmp->is_catalogue == false) {
            return;
        }


        if (tmp->files_and_catalogues_on_next_lvl == NULL) {

            node *new_node = create_node(name, begin_of_file, is_catalogue);
            tmp->files_and_catalogues_on_next_lvl = new_node;
            return;
        }

        //сделать проерку на каталог
        tmp = tmp->files_and_catalogues_on_next_lvl;
        while (tmp->files_and_catalogues_on_curr_lvl != NULL) {

            tmp = tmp->files_and_catalogues_on_curr_lvl;
        }

        node *new_node = create_node(name, begin_of_file, is_catalogue);

        tmp->files_and_catalogues_on_curr_lvl = new_node;
    }

    int get_begin_of_file(string way) {
        vector <string> s1 = parse_way(way);
        if (s1.size() == 0) {
            return -1;
        }

        node *tmp;
        tmp = find_data_object(s1);
        if (tmp == NULL) {
            return -1;
        }

        return tmp->begin_of_file;
    }

    void delete_file(string way, string name) {
        vector <string> s1 = parse_way(way);
        if (s1.size() == 0) {
            return;
        }

        node *tmp;
        tmp = find_data_object(s1);
        if (tmp == NULL) {
            return;
        }

        if (tmp->files_and_catalogues_on_next_lvl == NULL) {
            return;
        }

        node *prev = tmp;
        tmp = tmp->files_and_catalogues_on_next_lvl;

        while (tmp != NULL) {
            if (tmp->name._Equal(name)) {
                break;
            }
            prev = tmp;
            tmp = tmp->files_and_catalogues_on_curr_lvl;
        }

        //пока что можно удалять только файлы
        if (tmp->is_catalogue == true) {
            return;
        }

        if (tmp == prev->files_and_catalogues_on_next_lvl) {
            prev->files_and_catalogues_on_next_lvl = tmp->files_and_catalogues_on_curr_lvl;
            delete (tmp);
            return;
        }

        prev->files_and_catalogues_on_curr_lvl = tmp->files_and_catalogues_on_curr_lvl;
        delete (tmp);
    }

    void save_() {
        ofstream fout;
        fout.open("file_hierarchy_save.txt");

        print_("root/", root->files_and_catalogues_on_next_lvl, fout);

        fout.close();
    }

    void load_() {
        ifstream f1;
        f1.open("file_hierarchy_save.txt", std::ifstream::in);
        if (!f1.is_open()) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }

        string way;
        string name;
        int begin_of_file;
        bool is_catalogue;

        while (!f1.eof()) {

            f1 >> way;
            f1 >> name;
            f1 >> begin_of_file;

            if (begin_of_file != -1) { is_catalogue = false; }
            else { is_catalogue = true; }

            create_file(begin_of_file, way, name, is_catalogue);
            way.clear();
        }
        f1.close();
    }

private:
    node *root;

    node *create_node(string name, int begin_of_file, bool is_catalogue = false) {
        node *new_node = new node;

        new_node->name = name;
        new_node->files_and_catalogues_on_curr_lvl = NULL;
        new_node->files_and_catalogues_on_next_lvl = NULL;
        new_node->is_catalogue = is_catalogue;
        new_node->begin_of_file = begin_of_file;

        return new_node;
    }

    //подаются пути вида root/ex1/ex2/ex3/
    vector <string> parse_way(string way) {
        int pos;
        vector <string> s1;

        while (way.find("/") != -1) {
            pos = way.find("/");
            if (pos == 0) {
                break;
            }

            s1.push_back(way.substr(0, pos));
            way.erase(0, pos + 1);
        }

        return s1;
    }

    //поиск каталогов и файлов с совпадающем названием
    node *find_data_object(vector <string> s1) {
        if (s1.size() == 0) {
            return NULL;
        }

        node *result;
        node *tmp;
        tmp = root;
        result = NULL;

        for (string i : s1) {
            result = search_on_current_lvl(i, tmp);
            if (result == NULL) {
                return NULL;
            }

            tmp = result;
            tmp = tmp->files_and_catalogues_on_next_lvl;
        }
        return result;
    }

    //поиск каталогов и файлов на текущем уровне вложенности
    node *search_on_current_lvl(string name, node *leaf) {
        if (leaf == NULL) {
            return NULL;
        }

        node *tmp;
        tmp = leaf;

        while (tmp != NULL) {
            if (tmp->name._Equal(name)) {
                //cout << "YES!! " << tmp->name <<endl;
                return tmp;
            } else {
                tmp = tmp->files_and_catalogues_on_curr_lvl;
            }
        }

        return NULL;
    }

    void print_(string way, node *leaf, ofstream &fout) {
        if (leaf == NULL) {
            return;
        }
        fout << way << endl << leaf->name
             << endl << leaf->begin_of_file << endl;

        if (leaf->files_and_catalogues_on_next_lvl != NULL) {
            print_(way + leaf->name + "/", leaf->files_and_catalogues_on_next_lvl, fout);
        }

        print_(way, leaf->files_and_catalogues_on_curr_lvl, fout);
    }
};

class Memory_m {
private:
    int list_of_indexes[200] = {0};
    char mass_of_memory_blocks[200];
    int size_of_mass = 200;

    vector<int> find_space(int size) {
        vector<int> way;
        if (size <= 0) {
            return way;
        }

        for (int i = 0; i < 200; i++) {
            if (list_of_indexes[i] == 0) {
                way.push_back(i);
            }
            if (way.size() == size) {
                return way;
            }
        }

        way.clear();
        return way;
    }

    vector<int> find_way(int begin_of_file) {
        vector<int> s1;
        if (begin_of_file < 0 & begin_of_file > size_of_mass) {
            return s1;
        }

        int i = begin_of_file;

        while (i != -1 & i <= size_of_mass) {
            s1.push_back(i);
            i = list_of_indexes[i];
        }

        return s1;
    }

public:

    //функция возвращает начало файла
    int write(char text[], int size_of_text) {
        int size = size_of_text;

        vector<int> way = find_space(size);

        if (way.size() != size & way.size() == 0) {
            return -1;
        }

        for (int i = 0; i < way.size(); i++) {
            mass_of_memory_blocks[way[i]] = text[i];
        }
        for (int i = 0; i < way.size() - 1; i++) {
            list_of_indexes[way[i]] = way[i + 1];
        }
        //значение -1 указывает на окончание записи
        list_of_indexes[way.back()] = -1;

        return way[0];
    }

    vector<char> read(int begin_of_file) {
        vector<char> text;
        vector<int> way;

        way = find_way(begin_of_file);
        if (way.size() == 0) {
            return text;
        }

        //обработка на неверный индекс есть в функции find_way
        for (int i : way) {
            text.push_back(mass_of_memory_blocks[i]);
        }

        return text;
    }

    void delete_m(int begin_of_file) {
        vector<int> way;

        way = find_way(begin_of_file);
        if (way.size() == 0) {
            return;
        }

        for (int i : way) {
            list_of_indexes[i] = 0;
        }
    }

    void save_() {

        ofstream ofs("mass_of_memory_blocks.txt", ios::binary);
        if (ofs.is_open()) {
            ofs.write(mass_of_memory_blocks, end(mass_of_memory_blocks) - begin(mass_of_memory_blocks));
            ofs.close();
        } else std::cerr << "Unable to open file" << std::endl;

        ofstream fout("list_of_indexes.txt");
        if (fout.is_open()) {
            for (int i : list_of_indexes) {
                fout << i << endl;
            }
            fout.close();
        } else std::cerr << "Unable to open file" << std::endl;
    }

    void load_() {

        ifstream fin("mass_of_memory_blocks.txt", ios::binary);
        if (fin.is_open()) {
            fin.read(mass_of_memory_blocks, size_of_mass);
            fin.close();
        } else std::cerr << "Unable to open file" << std::endl;


        ifstream f1;
        f1.open("list_of_indexes.txt", std::ifstream::in);
        if (!f1.is_open()) {
            cout << "Ошибка открытия файла!" << endl;
            return;
        }
        for (int i = 0; i < size_of_mass; i++) {
            if (f1.eof()) {
                break;
            }
            f1 >> list_of_indexes[i];
        }
        f1.close();
    }
};

class Controller {
private:
    File_hierarchy file_hierarchy;
    Memory_m memory_m;
public:

    void create_file(char text[], int size_of_text, string way, string name_file) {
        int pos = -1;
        pos = memory_m.write(text, size_of_text);

        if (pos == -1) {
            return;
        }

        //нужно узнать создался ли файл
        file_hierarchy.create_file(pos, way, name_file, false);
    }

    void create_catalogue(string way, string name_file) {
        //нужно узнать создался ли файл
        file_hierarchy.create_file(-1, way, name_file, true);
    }

    //указывать путь + имя_файла: //путь/имя_файла/
    void read_file(string way) {
        int begin_of_file;

        begin_of_file = file_hierarchy.get_begin_of_file(way);
        if (begin_of_file == -1) {
            return;
        }

        vector<char> text;
        text = memory_m.read(begin_of_file);

        for (char i : text) {
            cout << i;
        }
    }

    void delete_file(string way, string name) {
        //функцию нужно переделать, чтобы она принимала еще и имя
        string way_and_name = way + "/" + name + "/";
        int pos = file_hierarchy.get_begin_of_file(way_and_name);

        file_hierarchy.delete_file(way, name);
        memory_m.delete_m(pos);
    }

    void save_() {
        file_hierarchy.save_();
        memory_m.save_();
    }

    void load_() {
        memory_m.load_();
        file_hierarchy.load_();
    }
};


int main() {

    char ae[] = {'h', 'e', 'y', 'k', 'i', 't'}; // 6
    char ae2[] = {'2', 'e', 'y', 'k', 'i'}; // 5
    char ae3[] = {'3', 'e', 'y', 'k',}; // 4
    Controller a;

    a.create_file(ae, 6, "root/", "text1");
    a.create_catalogue("root/", "qwe1");
    a.create_file(ae, 6, "root/qwe1/", "text4");
    a.create_file(ae2, 5, "root/", "text2");
    a.create_file(ae3, 4, "root/", "text3");
    a.create_catalogue("root/", "qwe2");
    a.create_file(ae3, 4, "root/qwe2/", "text5");

    a.read_file("root/qwe1/text4/");

    //a.save_tree();
    //a.load_();
    //a.read_file("root/qwe2/text5/");
    //a.save_();

    /*
    a.read_file("root/text2/");
    cout << endl;
    a.delete_file("root/", "text2");

    a.create_file(ae, 6, "root/", "text4");
    a.read_file("root/text4/");
    a.delete_file("root/", "text4");

    a.read_file("root/text3/");
    */


}
