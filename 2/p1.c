#include <unistd.h>
#include <stdio.h>

int main(int argc, char* args[]) {
    printf("%s \n", args[0]);
    // Unreachable
    _exit;
return 1;
}
