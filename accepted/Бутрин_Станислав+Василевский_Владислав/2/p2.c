#include <unistd.h>
#include <stdio.h>

int main(int argc, char* args[]) {
    FILE* f = fopen("test", "wr");
    fprintf(f, "%s", args[0]);
    fclose(f);
    // Unreachable
    _exit;
return 1;
}
