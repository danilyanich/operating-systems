#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include "memory.h"

/*
 * Вариант 11. Драпун Артём, Астапович Ольга
 *
 * Реализовать менеджер памяти со странично-сегментным разбиением, использующий принципы пространственной
 * локальности (см. раздел кэш-памяти) для загрузки виртуальных страниц в физическую память.
 * */

/* Так как я зафакапил историю коммитов, я решил добавить добавить комментарии ко всему, чтобы было ясно, что и как
 * работает
 * */

/*Размер кэша в страницах*/
#define CACHE_SIZE_IN_PAGES 1

/*Количество страниц которые будут загружаться помимо тех что требуется "(страницы пространственной локаьности)"*/
#define LOCALITY_SIZE 2

/*Размер страницы в байтах*/
#define PAGE_SIZE_IN_BYTES 8

/* Фактически имеется три главных "слоя памяти": физическая (основная, main), виртуальная и кэш-память
 * В физической памяти фактически и находятся данные, один сегмент в ней может быть разбит на большое количество кусков
 * (для экономии памяти), скажем (seg_1_part_1, seg_1_part_2, seg_1_part_3). При этом гаранитируется, что кусок с
 * номером X будет идти после куска с номером X - 1, (но не непосредственно после него - между ними может быть пустое
 * пространство или куски других сегментов). Размер выделенного сегмента НЕ подгоняется, чтобы быть кратным размеру страницы.
 * Память занимается оптимальным (в плане экономии места) способом.
 *
 * В виртуальной памяти сегмент с адресом (в вирт. памяти) address и размером size, всегда занимает память с address по
 * address + size (не включая). То есть в отличии от физической памяти, здесь сегмент идет сразу весь и не может разбиваться.
 * Виртуальная память, как и полагается, не существует. Зачем она здесь нужна? Чтобы иметь возможность реализовать pointer
 * arithmetic.
 *
 * Кэш состоит строго из определенного количества целых страниц. Каждая страница может быть выгружена/загружена отдельно.
 * Запись и чтение (инициируемое пользователем) всегда происходит в/из кэша и никогда не из/в физической памяти.
 * */

/* Структура описывающая узел двунаправленного списка, используется как тип операнда в функциях для работы со списками
 * */
struct Node {
    /* Пред. узел*/
    struct Node* previous;

    /* След. узел*/
    struct Node* next;
};

/* Структура описывающая узел сегмента в физической памяти. Один узел - один сегмент.*/
struct MainMemoryIdNode {
    /* След. узел*/
    struct MainMemoryIdNode* next;

    /* Пред. узел*/
    struct MainMemoryIdNode* previous;

    /* Указатель на начало сегмента в виртуальной памяти. Несмотря, на то, что он имеет тип char* в памяти программы он
     * никуда не указывает*/
    char* fromVirtualAddressPointer;

    /* Размер сегмента в байтах*/
    unsigned sizeInBytes;

    /* Корень двунаправленного списка который описывает страницы этого сегмента загруженные в кэш*/
    struct CachedPage* cachedPages;
};

/* Структура описывающая узел физической памяти. Один узел - одна ЧАСТЬ (не страница/не сегмент) какого-то сегмента*/
struct MainMemoryNode {
    /* Пред. узел*/
    struct MainMemoryNode* previous;

    /* След. узел */
    struct MainMemoryNode* next;

    /* Указатель на описание сегмента*/
    struct MainMemoryIdNode* mainMemoryIdNode;

    /* Начало этой части сегмента в физической памяти*/
    unsigned fromPhysicalAddress;

    /* Размер этой части сегмента в байтах*/
    unsigned sizeInBytes;
};

/* Структура описывающая страницу сегмента загруженную в кэш*/
struct CachedPage {
    /* Пред. узел*/
    struct CachedPage* previous;

    /* След. узел*/
    struct CachedPage* next;

    /* Номер страницы кэша в которую загружена эта страницы*/
    unsigned cachePageNumber;

    /* Номер страницы сегмента, которая загружена в кэш*/
    unsigned pageNumber;
};

/* Структура описываюшая слот/страницу кэша */
struct CachePage {
    /* Указатель на описание сегмента, страница которого загружена в эту страницу кэша (NULL - если пусто)*/
    struct MainMemoryIdNode* id;

    /* Указатель на описание страницы загруженной в кэш*/
    struct CachedPage* cachedPage;

    /* Флаги страницы кэша. Формат: XXXXXXXC, где X - зарезервировано, C - флаг описывающий были ли в страницу
     * (в её версию в кэше) внесены изменения после того, как она была загружена в кэш (1 - да были, 0 - иначе)*/
    unsigned char flags;
};

/* Список описаний сегментов. Важно то, что описания сегментов хранятся в порядке определяемом виртуальным адресом
 * начала сегмент (то ли по убыванию/то ли по возрастанию - я не помню:))*/
struct MainMemoryIdNode* _g_main_memory_ids_table;

/* Таблица описания страниц кэша*/
struct CachePage _g_cache_table[CACHE_SIZE_IN_PAGES];

/* Таблица частей сегментов физической памяти*/
struct MainMemoryNode* _g_main_memory_table = NULL;

/* Непосредственно кэш*/
char _g_cache_memory[CACHE_SIZE_IN_PAGES * PAGE_SIZE_IN_BYTES];

/* Непосредственно физическая память*/
char* _g_main_memory = NULL;

/* Общий размер физ. памяти*/
unsigned long _g_main_memory_size;

/* Размер использованной физ. памяти*/
unsigned long _g_used_memory_size;

/* Общая функция для работы с двунаправленными списками - удаление узла. Использует, своего рода, "C-наследование", когда
 * вместо Node* подставляется указатель на любую структуру, начало declaration которой совпадает с declaration Node.*/
void remove_node(struct Node* node, struct Node** root) {
    if (node->previous) {
        node->previous->next = node->next;

        if (node->next)
            node->next->previous = node->previous;
    } else {
        *root = node->next;

        if (node->next)
            node->next->previous = NULL;
    }
}

/* Общая функция для работы с двунаправленными списками - вставка узла.*/
void insert_new_node(struct Node* node_anchor,
                     struct Node* node_to_insert, char insert_after_anchor, struct Node** root) {
    if (insert_after_anchor) {
        if (node_anchor->next)
            insert_new_node(node_anchor->next, node_to_insert, 0, root);
        else {
            //после последнего элемента
            node_anchor->next = node_to_insert;

            node_to_insert->previous = node_anchor;
            node_to_insert->next = NULL;
        }
    } else {
        //перед anchor
        struct Node* another_side = node_anchor->previous;

        node_anchor->previous = node_to_insert;

        node_to_insert->previous = another_side;
        node_to_insert->next = node_anchor;

        if (another_side)
            another_side->next = node_to_insert;
        else
            *root = node_to_insert;
    }
}

/* Функция которая умеет читать из сегмента/писать в сегмент. При этом адресация происходит с помощью виртуальных
 * адресов, а не физических. Функция сама находит все части сегмента в которых лежит необходимая информация.*/
void make_transit_with_main_memory(struct MainMemoryIdNode* id, unsigned virtual_address,
        unsigned size, char* buffer, char read_or_write) {
    //0 - чтение. Читает `size` байт начиная с `virtual_address` в `buffer`
    //1 - запись. Записывает `size` байт из `buffer` в память начиная с `virtual_address`
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    unsigned last_virtual_address = id->fromVirtualAddressPointer - (char*)0;
    unsigned maximal_available_space = id->sizeInBytes - (virtual_address - last_virtual_address);
    unsigned total_bytes_to_transit = size <= maximal_available_space ? size : maximal_available_space;
    unsigned bytes_to_transit = total_bytes_to_transit;

    while (bytes_to_transit) {
        if (current_main_memory_node->mainMemoryIdNode == id) {
            unsigned from_virtual_address = last_virtual_address;
            unsigned to_virtual_address = last_virtual_address + current_main_memory_node->sizeInBytes;

            if ((virtual_address >= from_virtual_address && virtual_address < to_virtual_address) || bytes_to_transit != total_bytes_to_transit) {
                unsigned offset = bytes_to_transit != total_bytes_to_transit ? 0 : virtual_address - from_virtual_address;

                unsigned available = to_virtual_address - from_virtual_address - offset;
                unsigned bytes_to_transit_at_once = bytes_to_transit > available ? available : bytes_to_transit;

                char* buffer_ptr = buffer + (total_bytes_to_transit - bytes_to_transit);
                char* memory_ptr = _g_main_memory + current_main_memory_node->fromPhysicalAddress + offset;

                if (read_or_write) //write to mem
                    bcopy(buffer_ptr, memory_ptr, bytes_to_transit_at_once);
                else //read to buffer
                    bcopy(memory_ptr, buffer_ptr, bytes_to_transit_at_once);

                bytes_to_transit -= bytes_to_transit_at_once;
            }

            last_virtual_address = to_virtual_address;
        }

        current_main_memory_node = current_main_memory_node->next;
    }
}

/* Инициализирует страницу кэша как пустую*/
void free_cache_page(unsigned cache_page_number) {
    _g_cache_table[cache_page_number] = (struct CachePage){NULL, 0, 0};
}

/* Удаляет страницу загруженную в кэш из кэша. При этом страница кэша устанавливается как пустая. А соотв. узел из
 * списка загруженных в кэш страниц сегмента - удаляется*/
void free_cached_page(struct MainMemoryIdNode* main_memory_id_node, struct CachedPage* cached_page) {
    remove_node(cached_page, &main_memory_id_node->cachedPages);

    free_cache_page(cached_page->cachePageNumber);
    free(cached_page);
}

/* Выгрузка страницы из кэша. Происходит запись всех изменений (внесенных в страницу в кэше) в физическую память, а затем
 * удаление страницы из кэша.*/
void unload_cache_page(unsigned cache_page_number) {
    struct CachePage* cache_page = _g_cache_table + cache_page_number;
    struct MainMemoryIdNode* id = cache_page->id;

    if (id) {
        if (cache_page->flags & 0b1u)
            make_transit_with_main_memory(id, id->fromVirtualAddressPointer - (char*)0 +
                                              cache_page->cachedPage->pageNumber * PAGE_SIZE_IN_BYTES, PAGE_SIZE_IN_BYTES,
                                          _g_cache_memory + cache_page_number * PAGE_SIZE_IN_BYTES, 1);
        free_cached_page(id, cache_page->cachedPage);
    }
}

/* Так как лабораторная писалась с pointer arithmetic в голове, то предполагалось, что при записи/чтении пользователь
 * может передать не оригинальный (возвращённый из m_malloc) m_id, а, например, m_id + 10. Соответственно необходимо
 * уметь определять к какому сегменту относится тот или иной адрес виртуальной памяти, чем и занимается данная функция.
 * */
struct MainMemoryIdNode* determine_id_of_address(unsigned address) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

    while (current_main_memory_id_node) {
        unsigned from_virtual_address = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

        if (address >= from_virtual_address
            && address < from_virtual_address + current_main_memory_id_node->sizeInBytes)
            return current_main_memory_id_node;

        current_main_memory_id_node = current_main_memory_id_node->previous;
    }

    return NULL;
}

/* Удалить узел описыающий часть сегмента из памяти*/
void free_main_memory_node(struct MainMemoryNode* main_memory_node) {
    remove_node(main_memory_node, &_g_main_memory_table);

    free(main_memory_node);
}

/* Деаллоцировать сегмент. Также автоматически происходит удаление из кэша всех страниц этого сегмента загруженных в кэш
 * (без выгрузки в физ. память, сразу удаление). Удаление всех узлов описывающих части сегмента и узел-описание сегмента.*/
void free_segment(struct MainMemoryIdNode* main_memory_id_node) {
    //cache + cache pages
    struct CachedPage* current_cached_page = main_memory_id_node->cachedPages;

    while (current_cached_page) {
        struct CachedPage* next_cached_page = current_cached_page->next;

        free_cached_page(main_memory_id_node, current_cached_page);

        current_cached_page = next_cached_page;
    }

    //main memory nodes
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    while (current_main_memory_node) {
        if (current_main_memory_node->mainMemoryIdNode == main_memory_id_node)
            free_main_memory_node(current_main_memory_node);

        current_main_memory_node = current_main_memory_node->next;
    }

    //main memory id node
    remove_node(main_memory_id_node, &_g_main_memory_ids_table);
}

/* Просто функция*/
unsigned calculate_to_address(struct MainMemoryIdNode* main_memory_id_node) {
    return main_memory_id_node->fromVirtualAddressPointer - (char*)0 + main_memory_id_node->sizeInBytes;
}

/* Функция которая умеет распологать сегмент необходимого размера в виртуальной памяти. Самый тривиальный способ -
 * это взять максимальный занятый адрес в вирт. памяти, добавить к нему 1 и использовать. Но так при например 1000000
 * аллоцирований и след. за каждым из них деаллоцирований, unsigned переполнится и произойдет что-нибудь плохое.
 * Поэтому фактически вся эта функция нужна для экономии несуществующей виртуальной памяти. А именно, она ищет в
 * промежутках между другими сегментами пустой кусок подходящего размер и если находит - то занимает его, иначе -
 * делает так как написано в самом начале. Вовзращает виртуальный адрес на котором находится сегмент*/
char* place_id_node(struct MainMemoryIdNode* main_memory_id_node) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;
    unsigned size_of_chunk = main_memory_id_node->sizeInBytes;

    if (!current_main_memory_id_node) {
        //таблица - пуста
        _g_main_memory_ids_table = main_memory_id_node;

        return 0;
    } else {
        struct MainMemoryIdNode* maximal_address_node = current_main_memory_id_node;
        unsigned last_from = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

        current_main_memory_id_node = current_main_memory_id_node->previous;

        while (current_main_memory_id_node) {
            unsigned current_to = calculate_to_address(current_main_memory_id_node);
            long gap = (long)last_from - current_to;

            if (gap >= size_of_chunk) {
                //после какого-то из узлов
                insert_new_node(current_main_memory_id_node, main_memory_id_node, 0, &_g_main_memory_ids_table);
                return (char*)calculate_to_address(current_main_memory_id_node);
            }

            last_from = current_main_memory_id_node->fromVirtualAddressPointer - (char*)0;

            if (!current_main_memory_id_node->previous && last_from >= size_of_chunk) {
                //перед всеми узлами
                main_memory_id_node->next = current_main_memory_id_node;
                main_memory_id_node->previous = NULL;

                current_main_memory_id_node->previous = main_memory_id_node;

                return 0;
            }

            current_main_memory_id_node = current_main_memory_id_node->previous;
        }

        //после всех узлов
        insert_new_node(maximal_address_node, main_memory_id_node, 0, &_g_main_memory_ids_table);
        return (char*)calculate_to_address(maximal_address_node);
    }
}

/* Аллокация сегмента. Фактически сегмент просто заполняет все пустые места с самого начала физической памяти
 * (заполняет до тех пор пока не займет `size_of_chunk байт`). Тут есть пять вариантов ('-' - пустая память,
 * A/B - части сегментов):
 * 1) ---AB... - между началом и первым сегментом есть место
 * 2) AB... - между двумя частями сегментов нет места
 * 3) A---B... - между двумя частями сегментов есть место
 * 4) none - таблица пуста
 * 5) ...AB----- - после последнего сегмента есть место
 *
 * Каждый из них обрабатывается
 * */
void allocate_chunk(unsigned size_of_chunk, struct MainMemoryIdNode* main_memory_id_node) {
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;
    struct MainMemoryNode* last_main_memory_node = NULL;

    unsigned memory_to_allocate = size_of_chunk;
    unsigned last_physical_address = 0;

    while (memory_to_allocate > 0) {
        unsigned gap_from_last_node = current_main_memory_node
                                      ? current_main_memory_node->fromPhysicalAddress - last_physical_address
                                      : ~0u;

        char autoFollow = 0;

        if (gap_from_last_node > 0) {
            //после последнего узла есть свободное место
            unsigned nodeSize = gap_from_last_node >= memory_to_allocate
                                ? memory_to_allocate
                                : gap_from_last_node;

            memory_to_allocate -= nodeSize;

            struct MainMemoryNode* new_main_memory_node = malloc(sizeof(struct MainMemoryNode));

            new_main_memory_node->sizeInBytes = nodeSize;
            new_main_memory_node->mainMemoryIdNode = main_memory_id_node;
            new_main_memory_node->fromPhysicalAddress = last_physical_address;

            if (current_main_memory_node) {
                insert_new_node(current_main_memory_node, new_main_memory_node, 0, &_g_main_memory_table);

                autoFollow = 1;
            } else {
                if (last_physical_address)
                    //вставить после всех узлов
                    insert_new_node(last_main_memory_node, new_main_memory_node, 1, &_g_main_memory_table);
                else
                    //таблица - пуста
                    _g_main_memory_table = new_main_memory_node;
            }

            if (!autoFollow)
                current_main_memory_node = new_main_memory_node;
        } else
            autoFollow = 1;

        assert(current_main_memory_node != NULL);
        last_physical_address = current_main_memory_node->fromPhysicalAddress + current_main_memory_node->sizeInBytes;

        last_main_memory_node = current_main_memory_node;

        if (autoFollow)
            current_main_memory_node = current_main_memory_node->next;
    }
}

/* Выбирает страницу кэша для того чтобы загрузить в неё что-нибудь. В начале ищутся пустые страницы, если таких нет -
 * выбирается случайная страница*/
unsigned select_cache_page_to_use() {
    for (unsigned p = 0; p < CACHE_SIZE_IN_PAGES; ++p) {
        if (!(_g_cache_table + p)->id)
            return p;
    }

    return rand() % CACHE_SIZE_IN_PAGES;
}

/* Загрузка страницы сегмента `page_of_segment` в страницу кэша `cache_page_number`. Добавляется узел в список загруженнных
 * в кэш страниц сегмента. Осуществляется тразит данных страницы из физ. памяти в кэш*/
void load_cache_page(struct MainMemoryIdNode* main_memory_id_node, unsigned cache_page_number, unsigned page_of_segment) {
    struct CachedPage* cached_page = (struct CachedPage*)malloc(sizeof(struct CachedPage));

    cached_page->cachePageNumber = cache_page_number;
    cached_page->pageNumber = page_of_segment;

    struct CachePage* cache_page = _g_cache_table + cache_page_number;

    cache_page->id = main_memory_id_node;
    cache_page->cachedPage = cached_page;

    if (main_memory_id_node->cachedPages)
        insert_new_node(main_memory_id_node->cachedPages, cached_page, 0,
                        &main_memory_id_node->cachedPages);
    else
        main_memory_id_node->cachedPages = cached_page;

    make_transit_with_main_memory(main_memory_id_node,
                                  main_memory_id_node->fromVirtualAddressPointer - (char *) 0 +
                                  PAGE_SIZE_IN_BYTES * page_of_segment, PAGE_SIZE_IN_BYTES,
                                  _g_cache_memory + PAGE_SIZE_IN_BYTES * cache_page_number, 0);
}

/* Функция которая абстрагирует в себе что-то типа "мне нужно чтобы в кэше была страница X сегмента Y, мне плевать
 * есть ли она уже там или нет, просто дай мне указатель на эту страницу". Т.е. если такая страница уже в кэше
 * то просто возвращается указатель на нее (именно для быстрой проверки наличия какой-либо страницы сегмента в кэше,
 * у каждого сегмента и существует список страниц загруженных в кэш), если нет - то она загружается.*/
struct CachePage* provide_cache_page(struct MainMemoryIdNode* main_memory_id_node, unsigned page_of_segment) {
    struct CachedPage* current_cached_page = main_memory_id_node->cachedPages, *target_page = NULL;

    while (current_cached_page) {
        if (current_cached_page->pageNumber == page_of_segment)
            target_page = current_cached_page;

        current_cached_page = current_cached_page->next;
    }

    if (!target_page) {
        unsigned cache_page_number = select_cache_page_to_use();
        struct CachePage* cache_page = _g_cache_table + cache_page_number;

        if (cache_page->id)
            unload_cache_page(cache_page_number);

        load_cache_page(main_memory_id_node, cache_page_number, page_of_segment);

        return cache_page;
    } else
        return _g_cache_table + target_page->cachePageNumber;
}

/* То же самое что make_transit_with_main_memory, только теперь с кэшем, а не физ. памятью. По-прежнему адреса - виртуальные.
 * Также именно тут и загружаются страницы для обеспечения пространственной локальности. Кстати, если необходимо записать
 * в память (с точки зрения пользователя (в кэш - фактически)) кусок больший чем весь кэш - такая ситуация обработается
 * абсолютно корректно (главное чтобы размер кэша был не меньше одной страницы).*/
void make_transit_with_cache(unsigned virtual_address, unsigned size, char* buffer, char read_or_write, m_err_code* error) {
    //0 - read
    //1 - write
    struct MainMemoryIdNode* id = determine_id_of_address(virtual_address);

    *error = M_ERR_OK;

    if (id) {
        unsigned local_address = virtual_address - (id->fromVirtualAddressPointer - (char*)0);

        if (id->sizeInBytes - local_address >= size) {
            unsigned first_affected_page = local_address / PAGE_SIZE_IN_BYTES;
            unsigned last_affected_page = (local_address + size - 1) / PAGE_SIZE_IN_BYTES;
            unsigned pages_remain = id->sizeInBytes / PAGE_SIZE_IN_BYTES +
                                    (id->sizeInBytes % PAGE_SIZE_IN_BYTES ? 1 : 0) - last_affected_page - 1;
            unsigned locality_pages_count = pages_remain > LOCALITY_SIZE ? LOCALITY_SIZE : pages_remain;

            unsigned bytes_to_transit = size;
            unsigned offset = local_address - first_affected_page * PAGE_SIZE_IN_BYTES;

            for (unsigned p = first_affected_page; p <= last_affected_page + locality_pages_count; ++p) {
                struct CachePage *page = provide_cache_page(id, p);

                if (p <= last_affected_page) {
                    unsigned available = PAGE_SIZE_IN_BYTES - offset;
                    unsigned bytes_to_transit_at_once = available > bytes_to_transit ? bytes_to_transit : available;

                    char *buffer_ptr = buffer + (size - bytes_to_transit);
                    char *cache_ptr = _g_cache_memory + page->cachedPage->cachePageNumber * PAGE_SIZE_IN_BYTES + offset;

                    if (read_or_write) {
                        bcopy(buffer_ptr, cache_ptr, bytes_to_transit_at_once);

                        page->flags |= 0b1u;
                    } else
                        bcopy(cache_ptr, buffer_ptr, bytes_to_transit_at_once);

                    bytes_to_transit -= bytes_to_transit_at_once;

                    offset = 0;
                }
            }
        } else
            *error = M_ERR_OUT_OF_BOUNDS;
    } else
        *error = M_ERR_INVALID_CHUNK;
}

/* Адаптер для allocate_chunk. Возвращает указатель на начало сегмента в виртуальной памяти, который, в последствие,
 * может быть использован для pointer arithmetic*/
m_id m_malloc(int size_of_chunk, m_err_code* error) {
    if (_g_main_memory_size - _g_used_memory_size < size_of_chunk) {
        *error = M_ERR_ALLOCATION_OUT_OF_MEMORY;

        return NULL;
    }

    struct MainMemoryIdNode* main_memory_id_node = malloc(sizeof(struct MainMemoryIdNode));

    main_memory_id_node->sizeInBytes = size_of_chunk;
    main_memory_id_node->fromVirtualAddressPointer = place_id_node(main_memory_id_node);

    allocate_chunk(size_of_chunk, main_memory_id_node);

    *error = M_ERR_OK;

    _g_used_memory_size += size_of_chunk;

    assert(main_memory_id_node != NULL);
    return main_memory_id_node->fromVirtualAddressPointer;
}

/* Адаптер для free_segment. Если сегмент уже был деаллоцирован или такого сегмента не существует, то будет возвращен
 * E_INVALID_CHUNK
 *
 * Примечание: чтобы сегмент деаллоцировался, необходимо передавать оригинальный/не изменнненый указатель возвращенный
 * из m_malloc*/
void m_free(m_id ptr, m_err_code* error) {
    struct MainMemoryIdNode* current_main_memory_id_node = _g_main_memory_ids_table;

    while (current_main_memory_id_node) {
        if (current_main_memory_id_node->fromVirtualAddressPointer == (char*)ptr) {
            free_segment(current_main_memory_id_node);

            _g_used_memory_size -= current_main_memory_id_node->sizeInBytes;
            *error = M_ERR_OK;

            return;
        }

        current_main_memory_id_node = current_main_memory_id_node->previous;
    }

    *error = M_ERR_INVALID_CHUNK;
}

/* Адаптер для make_transit_with_cache*/
void m_read(m_id read_from_id, void* read_to_buffer, int size_to_read, m_err_code* error) {
    make_transit_with_cache(read_from_id - (void *) 0, size_to_read, read_to_buffer, 0, error);
}

/* Адаптер для make_transit_with_cache*/
void m_write(m_id write_to_id, void* write_from_buffer, int size_to_write, m_err_code* error) {
    make_transit_with_cache(write_to_id - (void *) 0, size_to_write, write_from_buffer, 1, error);
}

/* Инициализация/сброс. Работает корректно, даже если память уже была иницилизирована (все указатели, разумеется,
 * инвалидируются). Кэщ заполняется нулями, но это не критично*/
void m_init(int number_of_pages, int size_of_page) {
    srand(time(NULL));

    if (_g_main_memory) {
        //освободить основную память
        free(_g_main_memory);

        //удлить все сегменты
        while (_g_main_memory_ids_table) {
            free_segment(_g_main_memory_ids_table);
        }
    }

    _g_main_memory_size = number_of_pages * size_of_page;

    _g_main_memory = NULL;
    _g_main_memory_table = NULL;
    _g_main_memory_ids_table = NULL;
    _g_main_memory = malloc(_g_main_memory_size);

    //unnecessary
    for (unsigned i = 0; i < _g_main_memory_size; ++i)
        _g_main_memory[i] = 0;
    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES * PAGE_SIZE_IN_BYTES; ++i)
        _g_cache_memory[i] = 0;
    //end of unnecessary

    _g_used_memory_size = 0;

    //очистить кэш
    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES; ++i)
        free_cache_page(i);
}

/* Вывод числа в бинарном виде*/
void print_binary(unsigned char data) {
    unsigned char result[8];

    for (unsigned i = 0; i < 8; ++i) {
        result[7 - i] = '0' + data % 2;

        data /= 2;
    }

    printf("%s", result);
}

/* Дамп памяти*/
void dump() {
    struct MainMemoryNode* current_main_memory_node = _g_main_memory_table;

    printf("--MEMORY DUMP BEGINNING--\nMEMORY:\n");

    while (current_main_memory_node) {
        printf("MMID: %p, VRTF: %d, MMSZ: %d, SIZE: %d, PHSF: %d\n",
               current_main_memory_node->mainMemoryIdNode,
               (unsigned)(current_main_memory_node->mainMemoryIdNode->fromVirtualAddressPointer - (char*)0),
               current_main_memory_node->mainMemoryIdNode->sizeInBytes, current_main_memory_node->sizeInBytes,
               current_main_memory_node->fromPhysicalAddress);

        printf("DATA: ");

        for (unsigned i = 0; i < current_main_memory_node->sizeInBytes; ++i)
            printf("%c", *(_g_main_memory + current_main_memory_node->fromPhysicalAddress + i));

        printf("\n");

        current_main_memory_node = current_main_memory_node->next;
    }

    printf("\nCACHE:\n");

    for (unsigned i = 0; i < CACHE_SIZE_IN_PAGES; ++i) {
        struct CachePage* cache_page = _g_cache_table + i;

        printf("MMID: %p, PAGE: %d, MMPG: %d, FLGS: ", cache_page->id, i,
               cache_page->cachedPage ? cache_page->cachedPage->pageNumber : 0);

        print_binary(cache_page->flags);

        printf("\nDATA: ");

        for (unsigned j = 0; j < PAGE_SIZE_IN_BYTES; ++j)
            printf("%c", *(_g_cache_memory + (PAGE_SIZE_IN_BYTES * i) + j));

        printf("\n");
    }

    printf("--MEMORY DUMP END--\n\n");
}