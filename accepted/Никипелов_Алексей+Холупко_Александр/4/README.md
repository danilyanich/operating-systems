# Operation Systems
## Вариант 5
Реализовать модель файловой системы и реализовать базовые команды просмотра файлов на диске – создание, удаление, копирование, перемещение, запись в файл, чтение файла.
Обеспечить создание дампа файловой системы таким образом, чтобы можно было просмотреть структуру файловой системы и содержимое файлов.
Иерархическая (дерево) файловая система, с физической организацией файла -  непрерывное размещение;
## Members
 Холупко Александр, Никипелов Алексей, группа 821701
## Description
Implemented a file system with a tree hierarchy and continuous file allocation. Used File entity as abstraction above file and directory.


### Generate Makefile:
```bash
cmake .
```
### Build project:
```bash
make
```
### Run project:
```bash
./file-system
```

### Run tests
```bash
./tests
```