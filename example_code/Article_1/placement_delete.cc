#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <map>
using namespace std;

class X {
public:
    X() : used(false) {
        buffer = (char*)calloc(sizeof(char), MAX_BUFFER_SIZE);
    }

    void* allocate(std::size_t sz) {
        bool tmp = used || (sz > MAX_BUFFER_SIZE ? true : false);
        used = true;
        std::cout << "Can" << (tmp ? " not " : " ") << "allocate memory!" << std::endl;
        return tmp ? nullptr : buffer; 
    }

    void deallocate(void *ptr) {
        used = false;
    }
private:
    const static int MAX_BUFFER_SIZE = 256;
    char *buffer;
    bool used;
};

void* operator new(std::size_t sz, X& x) {
    return x.allocate(sz);
}

void operator delete(void* ptr, X& x) {
    return x.deallocate(ptr);
}

int main() {
    X x;
    int *p1 = new(x) int;
    delete p1;

    int *p2 = new(x) int;

    return 0;
}
