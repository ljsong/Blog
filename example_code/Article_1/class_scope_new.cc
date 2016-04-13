#include <iostream>

class A {
public:
    void* operator new(std::size_t sz) {
        std::cout << "custom new for size " << sz << std::endl;
        return ::operator new(sz);
    }
};

int main() {
    A* p1 = new A;
    delete p1;

    return 0;
}
