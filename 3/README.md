# Operating Systems


## 821702 group
### Вариант синхронизации элементов - мьютексы.
### 5 вариант: Реализовать на основе принципов RPC механизм вычисления произвольной математической функции. Разработанная клиент-серверная архитектура на базе RPC должна позволять легкое добавление и удаление новых RPC-процедур. Для упрощения реализации будем считать что RPC-вызов должен осуществляться только из приложений реализованных на языке С++.
### Members:
- Хвесеня Дарья
- Дыкова Юлия

## Overview
Implementation of client-server function calculation sigmoid function using gRPC.
Using dependency manager Bazel for gRPC installation. 

## Building
To only build the server and client, run the following:
```
bazel build :func_calc_server
bazel build :func_calc_client
```

## Run
To build and run the server and client, run the following:
```
bazel run :func_calc_server
bazel run :func_calc_client
```
