# File-system
## Task
Реализовать модель файловой системы и реализовать базовые команды просмотра файлов на диске – создание, удаление, копирование, перемещение, запись в файл, чтение файла.
Обеспечить создание дампа файловой системы таким образом, чтобы можно было просмотреть структуру файловой системы и содержимое файлов.
Иерархическая (дерево) файловая система, с физической организацией файла -  непрерывное размещение;
## Author
 Ilya Hontarau, Maria Zhirko
## Description
Implemented a file system with a tree hierarchy and continuous file allocation. Used File entity as abstrcion above file and dicectory.
Used c++17 with string_view and move sematic(not good example of using move sematin:)).
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
