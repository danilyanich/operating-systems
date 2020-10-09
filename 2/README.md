# Consumer-producer problem
## Task
Создать консольное приложение, порождающее несколько потоков.
Первый поток высчитывает таблицу значений функции (можно взять
любую математическую функцию), второй поток осуществляет вывод
каждого значения в файл по мере его высчитывания. Третий поток ведет
лог процедуры обмена, записывая туда время получения значения функции и
время записи данного значения в файл. Каждая пара значений, полученная в
процессе вычисления, должна быть занесена в объект класса Point,
который может быть уничтожен только тогда, когда информация о нем
будет занесена в лог-файл. Обращение к объекту Point должно
происходить через потокобезопасный умный указатель.

## Author
 Ilya Hontarau, Maria Zhirko
## Description
This is a calculator of math function with parallel writing logs to stdout and function values to passed through cli args file.
Implementation of consumer(many)-producer(1) problem solve. It represents an array of syncronized queues,
so consumers don't have a problem with concurent access to same queue and less time of blocking. 

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
./bin -file [path to file]
```
