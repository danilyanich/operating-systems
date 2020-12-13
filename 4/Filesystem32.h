//
// Created by wcobalt on 12/11/20.
//

/*
 * Данный файл определяет интерфейс драйвера генерализованной 32-х битной файловой системы. Он не знает ничего про
 * конкретную имплементацию.
 *
 *  Студенты:
 *      Астапович Ольга
 *      Драпун Артём
 *      Кондеев Павел
 *  гр. 821702
 *
 *  Вариант 7: Иерархическая (дерево) файловая система, с физической организацией файла -  связанный список индексов;
 * */

#include <string_view>
#include <vector>
#include <string>
#include <cstdlib>

#ifndef INC_4_FILESYSTEM_H
#define INC_4_FILESYSTEM_H

struct Filesystem {
    uint8_t* beginning;
    uint32_t size_in_clusters;
    uint32_t cluster_size_in_bytes;
};

//it's assumed there's enough space on drive
Filesystem init_volume(uint32_t size_in_clusters, uint32_t cluster_size, uint8_t* drive_pointer);

void make_directory(Filesystem filesystem, std::string_view path);;

//only empty dirs!
void delete_directory(Filesystem filesystem, std::string_view path);

std::vector<std::string> get_files_list_in_directory(Filesystem filesystem, std::string_view path);

void create_file(Filesystem filesystem, std::string_view path);

void delete_file(Filesystem filesystem, std::string_view path);

void copy_file(Filesystem filesystem, std::string_view source, std::string_view destination);

void move_file(Filesystem filesystem, std::string_view source, std::string_view destination);

bool does_file_exist(Filesystem filesystem, std::string_view path);

void write_to_file(Filesystem filesystem, std::string_view file, uint8_t* buffer, uint32_t size, bool do_append);

void read_from_file(Filesystem filesystem, std::string_view file, uint8_t * buffer, uint32_t size);

unsigned long get_size_of_file(Filesystem filesystem, std::string_view file);

#endif //INC_4_FILESYSTEM_H
