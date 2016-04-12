#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>

/* void* operator new(std::size_t sz, bool b) {
    std::cout << "boolean value of b is: " << b << std::endl;
    return std::malloc(sz);
}*/

/*void *operator new(std::size_t sz, std::string str) {
    std::cout << "We got a string: " << str << std::endl;
    return std::malloc(sz);
}*/

void* operator new(std::size_t sz, void* buffer) {
    std::cout << "Return the buffer directly!" << std::endl;
    return buffer;
}

int main() {
    //int *p1 = new (true) int; 
    //int *p2 = new ("Hello World") int;
    //*p2 = 5;
    //std::cout << "value of p2 is: " << *p2 << std::endl;
    //delete p2;

    return 0;
}
