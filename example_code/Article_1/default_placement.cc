#include <cstdio>
#include <cstdlib>
#include <iostream>

int main() {
    char *buffer = (char*) calloc(sizeof(char), 256);
    printf("We got a pre-allocated memory, its address is: 0x%x\n", buffer);

    unsigned int *p1 = new(buffer) unsigned int;
    *p1 = 0x34333231;
    printf("Allocate an area to p1, its address is: 0x%x\n", p1);

    std::cout << "buffer is: " << buffer << std::endl;
    free(buffer);

    return 0;
}
