cd Server/cmake-build-debug
start Server.exe
cd ../..
cd Client/cmake-build-debug
TIMEOUT /T 5
start Client.exe -multiwindow
TIMEOUT /T 3
start Client.exe -multiwindow
TIMEOUT /T 2
start Client.exe -multiwindow
TIMEOUT /T 1
start Client.exe -multiwindow
TIMEOUT /T 4
start Client.exe -multiwindow