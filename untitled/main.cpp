#include <iostream>

int main() {
    void * _g_allocator_memory = NULL;
    _g_allocator_memory = malloc(50);
    for (int i  = 0; i < 5; i ++) {}
    return 0;
}
