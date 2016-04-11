#include <stdexcept>
#include <iostream>

// 如果placement delete函数定义在全局作用域还对吗？
struct X {
    X() { throw std::runtime_error(""); }
    // custom placement new
    static void* operator new(std::size_t sz, bool b) {
        std::cout << "custom placement new called, b = " << b << '\n';
        return ::operator new(sz);
    }
    // custom placement delete
    static void operator delete(void* ptr, bool b)
    {
        std::cout << "custom placement delete called, b = " << b << '\n';
        ::operator delete(ptr);
    }
};

void* operator new(std::size_t sz, bool b) {
    std::cout << "Custom placement new called, b = " << b << std::endl;
    return ::operator new(sz);
}

// 注意：没有placement delete expression, 但是有placement delete function
void operator delete(void* ptr, bool b) {
    std::cout << "Custom placement delete called, b = " << b << std::endl;
    ::operator delete(ptr);
}

int main() {
   try {
     // X* p1 = new (true) X;
     int *p2 = new (true) int;
     operator delete(p2, true); // this is very important
   } catch(const std::exception&) { }
}
