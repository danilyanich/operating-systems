//
// Created by wcobalt on 12/12/20.
//
#include <string>

/*
 * Данный файл определяет дополнительные функции специфичные ТОЛЬКО для драйвера конкретной ФС - THICK32
 *
 *  Студенты:
 *      Астапович Ольга
 *      Драпун Артём
 *      Кондеев Павел
 *  гр. 821702
 *
 *  Вариант 7: Иерархическая (дерево) файловая система, с физической организацией файла -  связанный список индексов;
 * */

//this header adds THICK32-specific dump function

#ifndef INC_4_THICK32_H
#define INC_4_THICK32_H

#include "Filesystem32.h"

std::string dump(Filesystem filesystem, bool do_print_fat, bool do_print_flt, bool do_print_rdl,
                 bool do_dumb_dump_service_areas, uint32_t clusters_to_print_count, std::string_view path);

#endif //INC_4_THICK32_H
