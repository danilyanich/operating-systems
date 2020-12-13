#include <iostream>

#include "Filesystem32.h"
#include "THICK32.h"

const unsigned long DRIVE_SIZE_IN_BYTES = 0x100000; //~1MB
const unsigned CLUSTER_SIZE_IN_BYTES = 0x40; //64 bytes
const unsigned VOLUME_SIZE_IN_CLUSTERS = 256;

/*В этом файле просто происходит эксплуатация ФС
 *
 *  Студенты:
 *      Астапович Ольга
 *      Драпун Артём
 *      Кондеев Павел
 *  гр. 821702
 *
 *  Вариант 7: Иерархическая (дерево) файловая система, с физической организацией файла -  связанный список индексов;
 * */

int main() {
    std::cout << "Drive size: " << DRIVE_SIZE_IN_BYTES << " bytes\n";

    auto* drive = new uint8_t[DRIVE_SIZE_IN_BYTES];

    Filesystem filesystem = init_volume(VOLUME_SIZE_IN_CLUSTERS, CLUSTER_SIZE_IN_BYTES, drive);

    //FIRST TEST
    std::cout << dump(filesystem, true, true, true, true,
            10, "");

    create_file(filesystem, "/some_file");
    create_file(filesystem, "/some_file2");
    create_file(filesystem, "/some_file3");
    make_directory(filesystem, "/dir/");

    std::cout << "File size: " << get_size_of_file(filesystem, "/some_file") << "\n";

    std::vector<std::string> files = get_files_list_in_directory(filesystem, "/");

    std::cout << "Files list:\n";

    for (auto& file : files)
        std::cout << file << "\n";

    create_file(filesystem, "/dir/some_file");

    std::cout << dump(filesystem, true, true, true, true,
            10, "/dir");

    uint8_t buffer[] = "Some string which purpose is to be written in a file";

    write_to_file(filesystem, "/some_file", buffer, sizeof(buffer) - 1, false);
    write_to_file(filesystem, "/some_file", buffer, sizeof(buffer), true);

    uint8_t buffer1[120];

    std::cout << dump(filesystem, true, true, true, true,
                      10, "/dir/");

    read_from_file(filesystem, "/some_file", buffer1, get_size_of_file(filesystem, "/some_file"));

    std::cout << "Read data: " << buffer1 << "\n";

    delete_file(filesystem, "/dir/some_file");
   // delete_directory(filesystem, "/dir");


    std::cout << "Does /some_file exists: " << does_file_exist(filesystem, "/some_file") << "\n";
    std::cout << "Does /dir/some_file exists: " << does_file_exist(filesystem, "/dir/some_file");

    copy_file(filesystem, "/some_file", "/dir/copy_of_some_file");
    std::cout << dump(filesystem, true, true, true, true,
                      10, "/dir");
    move_file(filesystem, "/dir/copy_of_some_file", "/dir/moved_cosf");

    uint8_t buffer2[120];

    std::cout << dump(filesystem, true, true, true, true,
                      10, "/dir");

    read_from_file(filesystem, "/dir/moved_cosf", buffer2, get_size_of_file(filesystem, "/dir/moved_cosf"));

    std::cout << "Read data: " << buffer2 << "\n";

    std::cout << dump(filesystem, true, true, true, true,
                      10, "/dir");

/*
    //SECOND TEST:

    create_file(filesystem, "/some_file");
    create_file(filesystem, "/some_file2");
    create_file(filesystem, "/some_file3");

    make_directory(filesystem, "/new_dir");

    create_file(filesystem, "/new_dir/file");

    uint8_t buffer[] = "Some string which purpose is to be written in a file";

    write_to_file(filesystem, "/some_file", buffer, sizeof(buffer), false);
    write_to_file(filesystem, "/some_file", buffer, sizeof(buffer), true);

    std::vector<std::string> files_in_root = get_files_list_in_directory(filesystem, "/");

    std::cout << "Before deletion:\n";

    for (const auto& file : files_in_root)
        std::cout << "File in root: " << file << "\n";

    delete_file(filesystem, "/some_file2");

    files_in_root = get_files_list_in_directory(filesystem, "/");

    std::cout << "\nAfter deletion:\n";

    for (const auto& file : files_in_root)
        std::cout << "File in root: " << file << "\n";

    delete_file(filesystem, "/new_dir/file");
    delete_directory(filesystem, "/new_dir");

    copy_file(filesystem, "/some_file", "/copy_of_some_file");
    move_file(filesystem, "/some_file", "/moved_some_file");

    unsigned long file_size = get_size_of_file(filesystem, "/moved_some_file");

    auto *buffer_for_reading = new uint8_t[file_size];

    read_from_file(filesystem, "/moved_some_file", buffer_for_reading, file_size);

    delete[] buffer_for_reading;

    std::cout << dump(filesystem, true, true, true, true,
                      10, "/");*/

    return 0;
}
