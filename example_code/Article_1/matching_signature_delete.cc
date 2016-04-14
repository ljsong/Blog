#include <iostream>
#include <new>
#include <exception>

struct X {
    X() {
        throw std::bad_alloc();
    }

    static void* operator new(std::size_t sz, const std::nothrow_t& tag) {
        std::cout << "Allocate " << sz << " bytes!" << std::endl;
        return ::operator new(sz);
    }

    static void operator delete(void *ptr, const std::nothrow_t& tag) {
        std::cout << "deallocate!" << std::endl;
        return ::operator delete(ptr);
    }
};

int main() {
    try {
        X *x = new(std::nothrow) X;
    } catch (const std::bad_alloc& e) {
        std::cout << "Captured bad_alloc exception!" << std::endl;
    }

    return 0;
}

