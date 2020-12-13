//
// Created by wcobalt on 12/11/20.
//

/*
 * Даннный файл реализует драйвер файловой системы THICK32, при этом даннный драйвер соответствует интерфейсу
 * определенному в Filesystem32.h
 * */

/*
 * Описывать каждую функцию не имеет смысла, т.к. текстом все не объяснишь. Поэтому я перечислю основные концепции,
 * возможности и недостатки THICK32.
 *
 * Разметка файловой системы следующая: HEADER FAT FLT RDL DATA_AREA, где
 *  HEADER - хедер файловой системы, содержащий информацию о ней (размер кластера, количество кластеров, версию и т.д.)
 *  FAT - File Allocation Table, таблица индексов, где записаны односвязные списки кластеров принадлежащих файлам
 *  FLT - File Length Table, таблица где в i-ой ячейке содержится длина файла, который начинается в i-ом кластере
 *  RDL - Root Directory List, таблица фиксированного размера, где хранятся файлы/директории (DL entries) наход-ся в корне
 *  DATA_AREA - место, где и находятся кластеры ФС
 *
 *  Система поддерживает иерархичность в виде дерева (все директории кроме корневой реализованы через файлы в DATA_AREA).
 *  Файл каждой директории содержит DL (Directory List) по форме идентичный RDL, с той лишь разницей, что DL может увеличиваться
 *  по мере необходимости. DL имеет следующую форму: DL_ENTRIES_COUNT [DL_ENTRY], где DL_ENTRY: FILENAME FLAGS INDEX
 *  Каждый файл/директория может иметь имя длиной до 33 символов
 *
 *  В каждой директории хранятся спец. файлы '.' (текущая директория) и '..' (родительская директория). В корневой
 *  директории отсутствует '..'.
 *
 *  Файл не может занимать меньше одного кластера.
 *
 *  Количество занимаемых файлом кластеров может уменьшаться/увеличиваться динамически.
 *
 *  THICK32 поддерживает следующие (все) возможности из интерфейса Filesystem32.h:
 *   > Создание файлов
 *   > Создание директорий
 *   > Удаление файлов
 *   > Удаление директорий (только пустых)
 *   > Перемещение (переименовывание) файлов
 *   > Копирование файлов
 *   > Чтение файлов
 *   > Запись в файлы (в режиме append и без него)
 *   > Получение длины содержимого файла
 *   > Проверка существования файла
 *   > Получение списка файлов в директории
 *
 *  Дополнительно поддерживается возможность дампа с большим количеством опций для кастомизации вывода.
 *
 *  На уровне драйвера THICK32 также умеет увеличивать и уменьшать размер файлов, получать информацию о ФС прямо с диска
 *  (это значит, например, что структуру Filesystem можно загрузить зная, где расположена ФС), проверять совместимость
 *  файловой системы на диске и используемого драйвера, определять является ли ФС на диске файловой системой THICK32.
 *
 *  Студенты:
 *      Астапович Ольга
 *      Драпун Артём
 *      Кондеев Павел
 *  гр. 821702
 *
 *  Вариант 7: Иерархическая (дерево) файловая система, с физической организацией файла -  связанный список индексов;
 * */

#include <stdexcept>
#include <strings.h>
#include <iomanip>
#include <iostream>
#include <regex>
#include "Filesystem32.h"
#include "THICK32.h"

const unsigned FILENAME_SIZE = 33;
const unsigned FLAGS_SIZE = 2;
using index_type = uint32_t;
const unsigned INDEX_SIZE = sizeof(INDEX_SIZE);
using length_type = uint64_t;
const unsigned LENGTH_SIZE = sizeof(length_type);
const unsigned DL_ENTRY_SIZE = FILENAME_SIZE + FLAGS_SIZE + INDEX_SIZE; //file_name flags index file_size
using dl_entries_count_type = uint32_t;
const unsigned DL_ENTRIES_COUNT_SIZE = sizeof(dl_entries_count_type);

const index_type INDEX_FREE = 0x0000;
const index_type FILE_END = 0xFFFF;

const unsigned RDL_SIZE = 8;
const unsigned MINIMAL_DIR_DL_ENTRIES_COUNT = 2;
const unsigned MINIMAL_DIR_DL_SIZE = DL_ENTRIES_COUNT_SIZE + DL_ENTRY_SIZE * MINIMAL_DIR_DL_ENTRIES_COUNT;
const unsigned SIZE_IN_CLUSTERS_HEADER_OFFSET = 8;
const unsigned CLUSTER_SIZE_HEADER_OFFSET = 12;

const uint8_t THICK32_DRIVER_VERSION = 1;
const unsigned SIGNATURE_SIZE = 7;
const char HEADER[] = {'T', 'H', 'I', 'C', 'K', '3', '2', THICK32_DRIVER_VERSION,
                       0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0};

const unsigned FAT_DUMP_COLUMN_SIZE = 5;
const unsigned FAT_DUMP_COLUMNS_COUNT = 2;
const unsigned DUMP_NUMBER_WIDTH = 5;

const char DIR_SEPARATOR = '/';
const char FILENAME_REGEX[] = "[a-zA-Z0-9_]+";
const char PATH_REGEX[] = "(\\/[^/\\n]+)*\\/?";
const char CURRENT_DIR_NAME[] = ".";
const char PARENT_DIR_NAME[] = "..";

struct THICK32Info {
    char signature[SIGNATURE_SIZE];
    uint8_t version;
    uint32_t size_in_clusters;
    uint32_t cluster_size_in_bytes;
};

struct DLEntry {
    char filename[FILENAME_SIZE];
    bool is_directory;
    bool is_root_directory;
    bool is_active;
    index_type index;
};

THICK32Info obtain_thick32_info(const uint8_t* beginning) {
    THICK32Info info{};

    for (unsigned sb = 0; sb < SIGNATURE_SIZE; ++sb)
        info.signature[sb] = *(beginning + sb);

    info.version = static_cast<unsigned>(*(beginning + SIGNATURE_SIZE));
    info.size_in_clusters = *reinterpret_cast<const uint32_t*>(beginning + SIZE_IN_CLUSTERS_HEADER_OFFSET);
    info.cluster_size_in_bytes = *reinterpret_cast<const uint32_t*>(beginning + CLUSTER_SIZE_HEADER_OFFSET);

    return info;
}

bool check_thick32_and_driver_compatibility(uint8_t thick32_version, uint8_t driver_version) {
    return thick32_version == driver_version;
}

uint64_t calculate_dl_size(uint32_t dl_entries_count) {
    return DL_ENTRIES_COUNT_SIZE + dl_entries_count * DL_ENTRY_SIZE;
}

uint8_t* find_fat(Filesystem filesystem) {
    return filesystem.beginning + sizeof(HEADER);
}

uint8_t* find_flt(Filesystem filesystem) {
    return find_fat(filesystem) + filesystem.size_in_clusters * INDEX_SIZE;
}

uint8_t* find_rdl(Filesystem filesystem) {
    return find_flt(filesystem) + filesystem.size_in_clusters * LENGTH_SIZE;
}

uint8_t* find_data_area(Filesystem filesystem) {
    return find_rdl(filesystem) + calculate_dl_size(RDL_SIZE);
}

length_type determine_length_of_file(Filesystem filesystem, DLEntry entry) {
    return *(reinterpret_cast<length_type*>(find_flt(filesystem)) + entry.index);
}

void set_length_of_file(Filesystem filesystem, DLEntry entry, length_type length) {
    *(reinterpret_cast<length_type*>(find_flt(filesystem)) + entry.index) = length;
}

DLEntry read_dl_entry(uint8_t* from_where) {
    uint8_t* flags = from_where + FILENAME_SIZE;
    uint8_t flags_2nd_byte = *(flags + 1);

    bool is_directory, is_root_directory, is_active;

    is_directory = flags_2nd_byte & 1u;
    is_active = static_cast<uint8_t>(flags_2nd_byte >> 1u) & 1u;
    is_root_directory = static_cast<uint8_t>(flags_2nd_byte >> 2u) & 1u;

    index_type index = *reinterpret_cast<index_type*>(flags + FLAGS_SIZE);

    DLEntry dl_entry = {{}, is_directory, is_root_directory, is_active, index};

    bcopy(from_where, dl_entry.filename, FILENAME_SIZE);

    return dl_entry;
}

void write_dl_entry(uint8_t* to_where, const DLEntry& dl_entry) {
    for (unsigned b = 0; b < FILENAME_SIZE; ++b)
        *(to_where + b) = dl_entry.filename[b];

    for (unsigned f = 0; f < FLAGS_SIZE; ++f)
        *(to_where + FILENAME_SIZE + f) = 0;

    uint8_t flags_2n_byte{};

    flags_2n_byte |= dl_entry.is_directory;
    flags_2n_byte |= static_cast<uint8_t >(dl_entry.is_active << 1u);
    flags_2n_byte |= static_cast<uint8_t >(dl_entry.is_root_directory << 2u);

    *(to_where + FILENAME_SIZE + 1) = flags_2n_byte;

    *reinterpret_cast<index_type*>(to_where + FILENAME_SIZE + FLAGS_SIZE) = dl_entry.index;
}

//FLAGS: 0000'0000'0000'0ABC, A - is_root_dir, B - is_active (is this DL entry "counts"),
//C - is_dir (when it's root_dir this is `1` too)

void place_dl(Filesystem filesystem, uint8_t* beginning, uint32_t count, bool is_root_directory,
              DLEntry current_dir, DLEntry parent_dir) {
    if ((is_root_directory && count == 0) || (!is_root_directory && count < 2))
        throw std::invalid_argument("DL entries count is too small");

    *reinterpret_cast<dl_entries_count_type*>(beginning) = is_root_directory ? 1 : 2;

    beginning += DL_ENTRIES_COUNT_SIZE; //it's bad but still

    DLEntry entry{{}, false, false, false, FILE_END};

    for (uint32_t dle = 0; dle < count; ++dle)
        write_dl_entry(beginning + dle * DL_ENTRY_SIZE, entry);


    //junk in name but it's still zero-terminated
    bcopy(CURRENT_DIR_NAME, current_dir.filename, sizeof(CURRENT_DIR_NAME));

    current_dir.is_active = true;
    current_dir.is_directory = true;

    write_dl_entry(beginning, current_dir);

    if (!is_root_directory) {
        bcopy(PARENT_DIR_NAME, parent_dir.filename, sizeof(PARENT_DIR_NAME));

        parent_dir.is_directory = true;
        parent_dir.is_active = true;

        write_dl_entry(beginning + DL_ENTRY_SIZE, parent_dir);
    }
}

Filesystem init_volume(uint32_t size_in_clusters, uint32_t cluster_size, uint8_t* drive_pointer) {
    Filesystem filesystem{drive_pointer, size_in_clusters, cluster_size};

    unsigned header_size = sizeof(HEADER);

    //copy signature
    for (unsigned b = 0; b < header_size; ++b)
        *(drive_pointer + b) = HEADER[b];

    *reinterpret_cast<uint32_t*>(drive_pointer + SIZE_IN_CLUSTERS_HEADER_OFFSET) = size_in_clusters;
    *reinterpret_cast<uint32_t*>(drive_pointer + CLUSTER_SIZE_HEADER_OFFSET) = cluster_size;

    uint64_t fat_size_in_bytes = size_in_clusters * INDEX_SIZE;

    //initialize FAT
    for (index_type i = 0; i < size_in_clusters; ++i)
        *(reinterpret_cast<index_type*>(drive_pointer + header_size) + i) = INDEX_FREE;

    uint64_t flt_size_in_bytes = size_in_clusters * LENGTH_SIZE;

    //initialize FLT
    for (index_type l = 0; l < size_in_clusters; ++l)
        *(reinterpret_cast<length_type*>(drive_pointer + header_size + fat_size_in_bytes) + l) = 0;

    //initialize Root Directory List (RDL)
    place_dl(filesystem, drive_pointer + header_size + fat_size_in_bytes + flt_size_in_bytes,
             RDL_SIZE, true, {{}, true, true, true, FILE_END},
             {});

    return filesystem;
}

std::vector<DLEntry> retrieve_dl_entries(uint8_t* dl, bool only_active, dl_entries_count_type dl_size) {
    std::vector<DLEntry> dl_entries;

    for (dl_entries_count_type dle = 0; dle < dl_size; ++dle) {
        DLEntry entry = read_dl_entry(dl + DL_ENTRIES_COUNT_SIZE + dle * DL_ENTRY_SIZE);

        if (entry.is_active || !only_active)
            dl_entries.emplace_back(entry);
    }

    return dl_entries;
}

index_type find_free_index(Filesystem filesystem) {
    auto *fat = reinterpret_cast<index_type*>(find_fat(filesystem));

    for (index_type i = 0; i < filesystem.size_in_clusters; ++i)
        if (*(fat + i) == INDEX_FREE)
            return i;

    throw std::runtime_error("Unable to find free index");
}

void grow_file(Filesystem filesystem, index_type* fat, index_type last_index, uint32_t add_count) {
    index_type current_index = last_index;

    for (uint32_t i = 0; i < add_count; ++i) {
        index_type next_index = find_free_index(filesystem);

        *(fat + current_index) = next_index;

        current_index = next_index;
    }

    *(fat + current_index) = FILE_END;
}

void shrink_file(index_type *fat, index_type last_required_index) {
    index_type current_index = last_required_index;

    while (*(fat + current_index) != FILE_END) {
        index_type next_index = *(fat + current_index);

        *(fat + current_index) = (current_index == last_required_index ? FILE_END : INDEX_FREE);

        current_index = next_index;
    }
}

//after execution the caller must change dl_entry's length field and set it to required_size
void transform_file_size(Filesystem filesystem, DLEntry dl_entry, uint32_t required_size) {
    uint32_t required_size_in_clusters = required_size / filesystem.cluster_size_in_bytes
                                         + (required_size % filesystem.cluster_size_in_bytes > 0 ? 1 : 0);
    uint32_t actual_size_in_clusters = 0;

    index_type last_index = 0;
    index_type last_required_index = 0; //index which would be last if the file has required size
    auto *fat = reinterpret_cast<index_type*>(find_fat(filesystem));
    index_type index_value = *(fat + dl_entry.index);
    index_type index = dl_entry.index;

    do {
        ++actual_size_in_clusters;

        if (actual_size_in_clusters == required_size_in_clusters)
            last_required_index = index;

        index_type next_index = (index_value != FILE_END ? *(fat + index) : FILE_END);

        if (next_index == FILE_END)
            last_index = index;

        index = next_index;

        if (index != FILE_END)
            index_value = *(fat + index);
    } while (index != FILE_END);

    if (actual_size_in_clusters > required_size_in_clusters)
        //shrink
        shrink_file(fat, last_required_index);
    else if (actual_size_in_clusters < required_size_in_clusters) {
        //grow
        uint32_t diff = required_size_in_clusters - actual_size_in_clusters;

        grow_file(filesystem, fat, last_index, diff);
    }
}

void transit_data(Filesystem filesystem, DLEntry dl_entry, uint8_t* buffer, uint64_t size,
                  bool do_read, uint32_t offset) {//not sure about size's type
    length_type length = determine_length_of_file(filesystem, dl_entry);

    if (length - offset < size)
        throw std::invalid_argument("File is not big enough to transit so many bytes");

    auto *fat = reinterpret_cast<index_type*>(find_fat(filesystem));

    uint64_t bytes_to_transit = size;
    uint32_t cluster_offset = offset;

    index_type cluster = dl_entry.index;

    while (bytes_to_transit > 0) {
        uint32_t available_bytes = filesystem.cluster_size_in_bytes - cluster_offset;
        uint32_t bytes_to_transit_at_once = bytes_to_transit > available_bytes
                                            ? available_bytes
                                            : bytes_to_transit;

        uint8_t *cluster_ptr = find_data_area(filesystem) + cluster * filesystem.cluster_size_in_bytes;

        if (do_read)
            bcopy(cluster_ptr + cluster_offset, buffer + (size - bytes_to_transit), bytes_to_transit_at_once);
        else
            bcopy(buffer + (size - bytes_to_transit), cluster_ptr + cluster_offset, bytes_to_transit_at_once);

        bytes_to_transit -= bytes_to_transit_at_once;
        cluster_offset = 0;

        index_type next_cluster = *(fat + cluster);

        if (next_cluster == FILE_END)
            break;
        else
            cluster = next_cluster;
    }

    if (bytes_to_transit > 0)
        throw std::runtime_error("Unexpected end of file");
}

//writes to file and before that the function transforms the file's size to needed one
void write_and_adapt(Filesystem filesystem, DLEntry dl_entry, uint8_t* buffer, length_type size, length_type offset) {
    transform_file_size(filesystem, dl_entry, size + offset);

    set_length_of_file(filesystem, dl_entry, size + offset);

    transit_data(filesystem, dl_entry, buffer, size, false, offset);
}

void check_path(std::string_view path) {
    if (!std::regex_match(path.begin(), path.end(),std::regex(PATH_REGEX)))
        throw std::invalid_argument("Invalid path \"" + std::string(path) + "\"");
}

std::vector<DLEntry> retrieve_directory_dl_entries(Filesystem filesystem, DLEntry dl_entry, bool only_active) {
    std::vector<DLEntry> dl_entries;

    length_type length = determine_length_of_file(filesystem, dl_entry);
    auto* dir_descriptor_content = new uint8_t[length];

    transit_data(filesystem, dl_entry, dir_descriptor_content, length, true, 0);

    dl_entries = retrieve_dl_entries(dir_descriptor_content, only_active,
                                     (length - DL_ENTRIES_COUNT_SIZE) / DL_ENTRY_SIZE);

    delete[] dir_descriptor_content;

    return dl_entries;
}

struct LookupResult {
    DLEntry entry;
    dl_entries_count_type number;
};

//@todo double check
LookupResult lookup_file_backend(Filesystem filesystem, std::string_view path, std::string_view original_path) {
    check_path(path);

    uint8_t* current_dl = find_rdl(filesystem);
    std::vector<DLEntry> dl_entries = retrieve_dl_entries(current_dl, false, RDL_SIZE);

    if (path[path.size() - 1] == DIR_SEPARATOR)
        path = path.substr(0, path.size() - 1);

    bool do_continue;

    do {
        do_continue = false;

        std::string_view name = "."; //for "/"
        bool last_name = true;

        if (!path.empty()) {
            size_t slash_position = path.find(DIR_SEPARATOR, 1);
            last_name = (slash_position == std::string_view::npos);
            size_t real_position = last_name ? path.size() : slash_position;

            name = path.substr(1, real_position - 1);

            path = path.substr(real_position);
        }

        for (dl_entries_count_type dle = 0; dle < dl_entries.size(); ++dle) {
            DLEntry dl_entry = dl_entries[dle];

            if (std::string(dl_entry.filename) == name && dl_entry.is_active) {
                if (last_name)
                    return {dl_entry, dle};
                else {
                    do_continue = true;

                    dl_entries = retrieve_directory_dl_entries(filesystem, dl_entry, false);

                    break;
                }
            }
        }
    } while (do_continue);

    throw std::invalid_argument("Unable to locate file at \"" + std::string(original_path) + "\"");
}

LookupResult lookup_file(Filesystem filesystem, std::string_view path) {
    return lookup_file_backend(filesystem, path, path);
}

bool check_filename_validness(std::string_view filename) {
    return filename != CURRENT_DIR_NAME && filename != PARENT_DIR_NAME
           && std::regex_match(filename.begin(), filename.end(), std::regex(FILENAME_REGEX));
}

//is_one_entry_extendable means that the function is allowed to put a new entry at the end of dl
struct EntryAddResult {
    bool was_extended;
    uint8_t* dl_entry_ptr;
};

EntryAddResult add_dl_entry_to_dl(uint8_t* dl, dl_entries_count_type dl_entries_count,
                                  DLEntry dl_entry, bool is_one_entry_extendable) {
    uint32_t dle_to_use = 0;

    bool used_inactive = false;

    for (dl_entries_count_type e = 0; e < dl_entries_count; ++e) {
        //check is_active flag alone
        //it kinda violates kinda encapsulation but it's effective though

        uint8_t* entry_ptr = dl + DL_ENTRIES_COUNT_SIZE + DL_ENTRY_SIZE * e;

        bool is_active = *(entry_ptr + FILENAME_SIZE + 1) & 0b010u;

        if (!is_active && !used_inactive) {
            dle_to_use = e;
            used_inactive = true;
        }

        if (is_active && !strcmp(reinterpret_cast<char*>(entry_ptr), dl_entry.filename))
            throw std::invalid_argument("There's already DL entry with such filename");
    }

    if (!used_inactive) {
        if (is_one_entry_extendable) {
            dle_to_use = dl_entries_count;
        } else
            throw std::runtime_error("There's not enough space to store new DL entry");
    }

    ++(*reinterpret_cast<dl_entries_count_type*>(dl)); //increase dl entries count

    uint8_t* dl_entry_ptr = dl + DL_ENTRIES_COUNT_SIZE + dle_to_use * DL_ENTRY_SIZE;

    write_dl_entry(dl_entry_ptr, dl_entry);

    return {!used_inactive, dl_entry_ptr};
}

void create_dl_entry(Filesystem filesystem, std::string_view path, bool is_dir, index_type assign_index) {
    if (path[path.size() - 1] == DIR_SEPARATOR) {
        if (is_dir)
            path = path.substr(0, path.size() - 1);
        else
            throw std::invalid_argument("Invalid path, err=2");
    }

    size_t last_slash_pos = path.rfind(DIR_SEPARATOR);

    if (last_slash_pos != std::string_view::npos) {
        LookupResult directory_lr = lookup_file(filesystem, path.substr(0, last_slash_pos + 1));
        DLEntry directory = directory_lr.entry;
        length_type dir_length = determine_length_of_file(filesystem, directory);

        if (directory.is_directory) {
            std::string_view filename = path.substr(last_slash_pos + 1);

            if (check_filename_validness(filename)) {
                DLEntry new_file{{}, is_dir, false,
                                 true, assign_index != FILE_END ? assign_index : find_free_index(filesystem)};

                bcopy(filename.data(), new_file.filename, filename.size());

                dl_entries_count_type dl_entries_count = RDL_SIZE;
                uint8_t* dl = find_rdl(filesystem);

                if (!directory.is_root_directory) {
                    dl_entries_count = (dir_length - DL_ENTRIES_COUNT_SIZE) / DL_ENTRY_SIZE;

                    dl = new uint8_t[dir_length + DL_ENTRY_SIZE];
                    std::fill(dl + dir_length, dl + dir_length + DL_ENTRY_SIZE, 0);

                    transit_data(filesystem, directory, dl, dir_length, true, 0);
                }

                EntryAddResult add_result
                        = add_dl_entry_to_dl(dl, dl_entries_count, new_file, !directory.is_root_directory);

                //take index
                if (assign_index == FILE_END)
                    *(reinterpret_cast<index_type*>(find_fat(filesystem)) + new_file.index) = FILE_END;

                //write dl if it's a dir
                if (is_dir) {
                    auto* dir_dl = new uint8_t[MINIMAL_DIR_DL_SIZE];
                    std::fill(dir_dl, dir_dl + MINIMAL_DIR_DL_SIZE, 0);

                    place_dl(filesystem, dir_dl, MINIMAL_DIR_DL_ENTRIES_COUNT, false,
                             {{}, true, false, true, new_file.index}, directory);

                    write_and_adapt(filesystem, new_file, dir_dl, MINIMAL_DIR_DL_SIZE, 0);

                    delete[] dir_dl;
                }

                if (!directory.is_root_directory) {
                    write_and_adapt(filesystem, directory, dl,
                                    dir_length + (add_result.was_extended ? DL_ENTRY_SIZE : 0), 0);

                    delete[] dl;
                }
            } else
                throw std::invalid_argument("Invalid DL entry's filename");
        } else
            throw std::invalid_argument("Invalid path, err=0");
    } else
        throw std::invalid_argument("Invalid path, err=1");
}

void make_directory(Filesystem filesystem, std::string_view path) {
    create_dl_entry(filesystem, path, true, FILE_END);
}

void deallocate(Filesystem filesystem, DLEntry entry) {
    auto* fat = reinterpret_cast<index_type*>(find_fat(filesystem));
    auto* flt = reinterpret_cast<length_type*>(find_flt(filesystem));

    index_type index_value = *(fat + entry.index);
    index_type index = entry.index;

    do {
        *(flt + index) = 0;

        index_type next_index = (index_value != FILE_END ? *(fat + index) : FILE_END);

        *(fat + index) = INDEX_FREE;

        index = next_index;
        index_value = *(fat + index);
    } while (index != FILE_END);
}

void delete_dl_entry(Filesystem filesystem, std::string_view path, bool do_deallocate) {
    size_t last_slash_pos = path.rfind(DIR_SEPARATOR);

    if (last_slash_pos != std::string_view::npos) {
        LookupResult directory_lr = lookup_file(filesystem, path.substr(0, last_slash_pos + 1));
        DLEntry directory = directory_lr.entry;
        length_type dir_length = determine_length_of_file(filesystem, directory);

        if (directory.is_directory) {
            LookupResult lr = lookup_file(filesystem, path);
            DLEntry entry = lr.entry;

            if (!entry.is_root_directory) {
                if (entry.is_directory &&
                    retrieve_directory_dl_entries(filesystem, entry, true).size() > MINIMAL_DIR_DL_ENTRIES_COUNT)
                    throw std::invalid_argument("The dir isn't empty");

                uint8_t *dl = find_rdl(filesystem);

                if (!directory.is_root_directory) {
                    dl = new uint8_t[dir_length];

                    transit_data(filesystem, directory, dl, dir_length, true, 0);
                }

                //it violates encapsulation but it's effective
                *(dl + DL_ENTRIES_COUNT_SIZE + lr.number * DL_ENTRY_SIZE + FILENAME_SIZE + 1) &= ~(0b010u);

                if (do_deallocate)
                    deallocate(filesystem, entry);

                if (!directory.is_root_directory) {
                    //check shrink availability
                    length_type total_dir_length = dir_length;

                    //@todo implement shrink (it's not necessary)

                    //flush dl
                    write_and_adapt(filesystem, directory, dl, total_dir_length, 0);

                    delete[] dl;
                }
            } else
                throw std::invalid_argument("It's impossible to delete root directory");
        }
    } else
        throw std::invalid_argument("Invalid path, err=0");
}

//only empty dirs!
void delete_directory(Filesystem filesystem, std::string_view path) {
    delete_dl_entry(filesystem, path, true);
}

std::vector<std::string> get_files_list_in_directory(Filesystem filesystem, std::string_view path) {
    DLEntry dl_entry = lookup_file(filesystem, path).entry;
    std::vector<DLEntry> dl_entries;

    if (dl_entry.is_root_directory)
        dl_entries = retrieve_dl_entries(find_rdl(filesystem), true, RDL_SIZE);
    else if (dl_entry.is_directory)
        dl_entries = retrieve_directory_dl_entries(filesystem, dl_entry, true);
    else
        throw std::invalid_argument("Passed path doesn't point to a directory");

    std::vector<std::string> filenames;

    for (const auto& entry : dl_entries)
        filenames.emplace_back(std::string(entry.filename));

    return filenames;
}

void create_file(Filesystem filesystem, std::string_view path) {
    create_dl_entry(filesystem, path, false, FILE_END);
}

void delete_file(Filesystem filesystem, std::string_view path) {
    delete_dl_entry(filesystem, path, true);
}

void copy_file(Filesystem filesystem, std::string_view source, std::string_view destination) {
    DLEntry source_entry = lookup_file(filesystem, source).entry;

    create_file(filesystem, destination);
    DLEntry dest_entry = lookup_file(filesystem, destination).entry;

    //just copy data
    length_type original_size = determine_length_of_file(filesystem, source_entry);
    auto *data = new uint8_t[original_size];

    transit_data(filesystem, source_entry, data, original_size, true, 0);

    write_and_adapt(filesystem, dest_entry, data, original_size, 0);
}

void move_file(Filesystem filesystem, std::string_view source, std::string_view destination) {
    DLEntry source_entry = lookup_file(filesystem, source).entry;

    if (!source_entry.is_directory) {
        size_t last_slash_pos = source.rfind(DIR_SEPARATOR);

        if (last_slash_pos != std::string_view::npos) {
            index_type index = source_entry.index;
            length_type length = determine_length_of_file(filesystem, source_entry);

            delete_dl_entry(filesystem, source, false);

            create_dl_entry(filesystem, destination, false, index);
            DLEntry dest_entry = lookup_file(filesystem, destination).entry;

            set_length_of_file(filesystem, dest_entry, length);
        } else
            throw std::invalid_argument("Invalid path, err=0");
    } else
        throw std::invalid_argument("You really thought you can just move a dir?? whatta hell man!? what's wrong with you!?");
}

bool does_file_exist(Filesystem filesystem, std::string_view path) {
    try {
        lookup_file(filesystem, path);
    } catch (std::invalid_argument& exc) {
        return false;
    }

    return true;
}

void write_to_file(Filesystem filesystem, std::string_view file, uint8_t* buffer, uint32_t size, bool do_append) {
    LookupResult lr = lookup_file(filesystem, file);
    DLEntry entry = lr.entry;

    write_and_adapt(filesystem, entry, buffer, size, do_append ? determine_length_of_file(filesystem, entry) : 0);
}

//it can read dirs too (it's a feature, ya'know)
void read_from_file(Filesystem filesystem, std::string_view file, uint8_t * buffer, uint32_t size) {
    DLEntry entry = lookup_file(filesystem, file).entry;

    transit_data(filesystem, entry, buffer, size, true, 0);
}

unsigned long get_size_of_file(Filesystem filesystem, std::string_view file) {
    return determine_length_of_file(filesystem, lookup_file(filesystem, file).entry);
}

void dump_dl_entry(Filesystem filesystem, std::ostream& ostream, const DLEntry& entry) {
    length_type length = entry.index != FILE_END ? determine_length_of_file(filesystem, entry) : 0;

    ostream << "    > " << entry.filename << " is_root: " << entry.is_root_directory << ", is_active: "
            << entry.is_active << ", is_dir: " << entry.is_directory
            << "; Index: " << (entry.is_active ? (entry.index != FILE_END ? std::to_string(entry.index) : "EOF") : "undef")
            << "; Length: " << (entry.is_active ? std::to_string(length) : "undef");
}

void dump_dl(Filesystem filesystem, std::ostream& ostream, std::vector<DLEntry>& dl_entries) {
    ostream << "DL entries count: " << dl_entries.size() << "\n";

    for (const auto &entry : dl_entries) {
        dump_dl_entry(filesystem, ostream, entry);

        ostream << "\n";
    }
}

void dumb_dump(std::ostream& ostream, const uint8_t* ptr, uint32_t count) {
    for (uint32_t b = 0; b < count; ++b)
        ostream << static_cast<char>(*(ptr + b));
}

std::string dump(Filesystem filesystem, bool do_print_fat, bool do_print_flt, bool do_print_rdl,
                 bool do_dumb_dump_service_areas, uint32_t clusters_to_print_count, std::string_view path) {
    std::ostringstream out;

    out << "----MEMORY DUMP BEGINS----\n";

    THICK32Info info = obtain_thick32_info(filesystem.beginning);

    bool signature_match = true;

    out << "Signature: ";

    for (unsigned sb = 0; sb < SIGNATURE_SIZE; ++sb) {
        char schar = info.signature[sb];

        out << schar;

        if (schar != HEADER[sb])
            signature_match = false;
    }

    out << " " << (signature_match ? "(matches)" : "(doesn't match)") << "\n";

    bool driver_compatibility = check_thick32_and_driver_compatibility(info.version, THICK32_DRIVER_VERSION);

    out << "THICK32 version: " << static_cast<unsigned>(info.version)
        << ", driver is for: " << static_cast<unsigned>(THICK32_DRIVER_VERSION)
        << ", " << (driver_compatibility ? "compatible" : "incompatible") << "\n";
    out << "Size in clusters: " << info.size_in_clusters << "\n";
    out << "Cluster size: " << info.cluster_size_in_bytes << "\n";

    if (do_dumb_dump_service_areas) {
        //dumb dump of header
        out << "Dumb dump: ";

        dumb_dump(out, filesystem.beginning, sizeof(HEADER));

        out << "\n";
    }

    if (do_print_fat) {
        out << "\nFAT:\n";

        auto *fat = reinterpret_cast<index_type*>(find_fat(filesystem));
        auto *flt = reinterpret_cast<length_type*>(find_flt(filesystem));

        for (uint32_t i = 0; i < filesystem.size_in_clusters; ++i) {
            uint32_t row_size = FAT_DUMP_COLUMN_SIZE * FAT_DUMP_COLUMNS_COUNT;

            if (i % row_size == 0)
                out << std::setw(DUMP_NUMBER_WIDTH) << i << ": ";
            else if (i % FAT_DUMP_COLUMN_SIZE == 0)
                out << "     ";//fill with DUMP_NUMBER_WIDTH spaces

            index_type index = *(fat + i);
            std::string index_representation;

            if (index == INDEX_FREE)
                index_representation = "FRE";
            else if (index == FILE_END)
                index_representation = "EOF";
            else
                index_representation = std::to_string(index);

            out << std::setw(DUMP_NUMBER_WIDTH) << index_representation;

            if (do_print_flt) {
                out << " ";
                out << std::setw(DUMP_NUMBER_WIDTH) << "(" + std::to_string(*(flt + i)) + ")";
            }

            if (i != filesystem.size_in_clusters - 1)
                out << ",";

            if ((i % row_size) == row_size - 1 || i == filesystem.size_in_clusters - 1)
                out << "\n";
        }

        //there's no dumb dump
    }

    if (do_print_rdl) {
        out << "\nRDL:\n";

        uint8_t* rdl = find_rdl(filesystem);

        std::vector<DLEntry> rdl_entries = retrieve_dl_entries(rdl, false, RDL_SIZE);

        dump_dl(filesystem, out, rdl_entries);

        if (do_dumb_dump_service_areas) {
            out << "Dumb dump: \n";
            out << "DLEC: ";

            dumb_dump(out, rdl, DL_ENTRIES_COUNT_SIZE);

            out << "\n";

            for (uint32_t rdle = 0; rdle < RDL_SIZE; ++rdle) {
                out << std::setw(DUMP_NUMBER_WIDTH) << rdle << ": ";

                dumb_dump(out, rdl + DL_ENTRIES_COUNT_SIZE + rdle * DL_ENTRY_SIZE, DL_ENTRY_SIZE);

                out << "\n";
            }
        }
    }

    if (!path.empty()) {
        DLEntry directory = lookup_file(filesystem, path).entry;

        if (directory.is_directory && !directory.is_root_directory) {
            out << "\nPath \"" << path << "\"'s DL:\n";

            std::vector<DLEntry> dl_entries = retrieve_directory_dl_entries(filesystem, directory, false);

            dump_dl(filesystem, out, dl_entries);
        }
    }

    if (clusters_to_print_count > 0) {
        out << "\n" << clusters_to_print_count << " first clusters dump:\n";

        for (uint32_t c = 0; c < clusters_to_print_count; ++c) {
            uint8_t *data_area = find_data_area(filesystem);

            out << std::setw(DUMP_NUMBER_WIDTH) << c << ": ";

            dumb_dump(out, data_area + filesystem.cluster_size_in_bytes * c, filesystem.cluster_size_in_bytes);

            out << "\n";
        }
    }

    out << "----MEMORY DUMP ENDS----\n";

    out.flush();

    return out.str();
}