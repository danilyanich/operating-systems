# File-system
## Task
Реализовать модель файловой системы и реализовать базовые команды просмотра файлов на диске – создание, удаление, копирование, перемещение, запись в файл, чтение файла.
Обеспечить создание дампа файловой системы таким образом, чтобы можно было просмотреть структуру файловой системы и содержимое файлов.
Иерархическая (дерево) файловая система, с физической организацией файла -  непрерывное размещение;
## Author
 Shepko Matvey, Shyshko Alexandr
## Description
Implemented a file system with a tree hierarchy and continuous file allocation. Used File entity as abstraction above file and directory.
Used c++17 with string_view and move semantic(not good example of using move sematic:)).
Mostly covered with tests, for test framework was used Catch, installed using cmake fetch content.


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
