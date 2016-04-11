# C++中的new和delete

前天在[cppreference.com](http://en.cppreference.com/w/ "cppreference.com")上查找资料时，经过层层的链接重定向，偶然翻到了[new expression](http://en.cppreference.com/w/cpp/language/new "new expression")的定义，进入页面前心里还有疑惑：“不应该是operator吗？new expression又是什么意思”。完整的阅读了new expression的介绍后，才发现自己对C++中new的理解很肤浅，所以特此记录下来，一是形成笔记时会再一次梳理内容，可以检验自己是否真正掌握了文章的意思；二是供以后查阅使用。<br />
全文按照如下章节组织(本文只讨论单对象内存的申请和释放，数组同理)：<br />
　　[1. new expression和operator new](#section_1) <br />
　　　　[1.1. new expression](#sub_section_1_1) <br />
　　　　[1.2. operator new](#sub_section_1_2) <br />
　　　　　　[1.2.1. replaceable allocation functions](#sub_sction_1_2_1) <br />
　　　　　　[1.2.2. placement allocation functions](#sub_sction_1_2_2) <br />
　　　　　　[1.2.3. class-specific allocation functions](#sub_sction_1_2_3) <br />
　　[2. delete expression和operator delete](#section_2) <br />
　　　　[2.1. delete expression](#sub_sction_2_1) <br />
　　　　[2.2. operator delete](#sub_sction_2_2) <br />
　　[3. 总结](#section_3) <br />

## <span id="section_1"> 1. new expression和operator new </span>
### <span id="sub_section_1_1"> 1.1 new expression </span>
new expression其实就是我们在申请内存时使用的表达式，其定义如下：

```C++
/* placement_params 将会在operator new函数章节详细讨论 */
1. ::(optional) new (placement_params)(optional) ( type ) initializer(optional)
2. ::(optional) new (placement_params)(optional) type initializer(optional)
```
两种定义的区别在于`type`是否包含小括号。通常情况下，我们使用第二种方式申请内存，例如：

```C++
1. int *a = new int(5);
2. char *b = new char;
3. auto *p = new auto('c');  // initializer is required
```
注意第三个例子中的`initializer`是**必须**的，C++语言需要它去推导出`p`的类型。
第一种定义一般用于申请比较复杂的类型时，需要用小括号描述一个完整的类型定义，例如：

```C++
1. new int(*[10]) ();		//wrong, parsed as (new int) (*[10]) ()
2. new (int (*[10]) ());	//correct, allocates an array of ten pointers to function
```
从整体上看，执行完new expression后，我们便获得了一块内存并且可以对这块内存进行一系列的操作。但是，在执行new expression时，C++语言做了哪些动作呢？下面看下C++语言是如何解释new expression的：
> The new expression attempts to **allocate storage** and then attempts to **construct and initialize** either a single unnamed object, or an unnamed array of objects in the allocated storage. The new-expression returns a prvalue pointer to the constructed object or, if an array of objects was constructed, a pointer to the initial element of the array

从上述解释中，我们可以看出new expression包括了两项动作：<br/>
　　i.  分配内存<br/>
　　ii. 构造并初始化对象<br/>
其中，分配内存的动作由`operator new`函数完成，构造并初始化对象则由构造函数完成。那么，我们可以得出这样的结论：new expression其实是通过调用`operator new`函数和相应类型的构造函数完成动作的，更形象的可以描述为：

```C++
new expression(T) {		// 只是用于形象的描述
    T* t = operator new(sizeof(T));
    t->consturctor_of_T();
}
```

### <span id="sub_section_1_2"> 1.2 operator new </span>
<span id="operator_new_define"> 首先，描述下`operator new`函数是如何定义的：</span>

```C++
// replaceable allocation functions
1. void* operator new (std::size_t count);
2. void* operator new (std::size_t count, const std::nothrow_t& tag);

// placement allocation functions
3. void* operator new (std::size_t count, void* ptr);
4. void* operator new(std::size_t count, user_defined_args...);

// class-specific allocation functions
5. void* T::operator new(std::size_t count);
6. void* T::operator new(std::size_t count, user_defined_args...);
```
上述6种`operator new`的定义可以分为3类进行讨论，其中每一类别中的两种定义又以是否抛出异常(std::bad_alloc或者是继承自std::bad_alloc的子类)区分，下面按照上述三种类别分别介绍：

#### <span id="sub_sction_1_2_1"> 1.2.1 Replaceable allocation functions </span>
顾名思义，这类`operator new`函数是可被替换的，当我们提供了一个与定义1或者2相同签名的`operator new`函数时，默认的实现就会被替换为用户自定义的版本。**注意**，当我们定义了多个相同签名的`operator new`函数或者是`operator new`函数被指定为`inline`时，编译器的行为是未知的。以下为测试代码：
```C++
#include <cstdio>
#include <cstdlib>

//replacement of a minimal set of functions:
void* operator new(std::size_t sz) {
    std::printf("global op new called, size = %zu\n", sz);
    return std::malloc(sz);
}

void operator delete(void* ptr) noexcept {
    std::puts("global op delete called");
    std::free(ptr);
}

int main() {
    int *p1 = new int;
    delete p1;

    int* p2 = new int[10];
    delete[] p2;
}
```
输出结果如下：

![Result of replaceable allocation](/home/allen/Pictures/example_code_replaceable_new.png)

####1.2.2 Placement allocation functions
placement new方法分为标准库形式([定义][1]3)和用户自定义形式两种([定义][1]4),

[1]: #operator_new_define "operator new的定义"