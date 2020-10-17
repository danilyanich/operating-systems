# Client-server functions calculator 

## Author
 Ilya Hontarau, Maria Zhirko

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
