# C++中的new和delete

　　前天在[cppreference.com](http://en.cppreference.com/w/ "cppreference.com")上查找资料时，经过层层的链接重定向，偶然翻到了[new expression](http://en.cppreference.com/w/cpp/language/new "new expression")的定义，进入页面前心里还有疑惑：“不应该是operator吗？new expression又是什么意思”。完整的阅读了new expression的介绍后，才发现自己对C++中new的理解很肤浅，所以特此记录下来，一是形成笔记时会再一次梳理内容，可以检验自己是否真正掌握了文章的意思；二是供以后查阅使用。
　　全文按照如下章节组织(本文只讨论单对象内存的申请和释放，数组同理)：
　　1. new expression和operator new
　　　　1.1. new expression
　　　　1.2. operator new
　　2. delete expression和operator delete
　　　　2.1. delete expression
　　　　2.2. operator delete
　　3. 总结

## 1. new expression和operator new
### 1.1 new expression
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
> The new expression attempts to ==allocate storage== and then attempts to ==construct== and ==initialize== either a single unnamed object, or an unnamed array of objects in the allocated storage. The new-expression returns a prvalue pointer to the constructed object or, if an array of objects was constructed, a pointer to the initial element of the array

从上述解释中，我们可以看出new expression包括了两项动作：
　　i.  分配内存
　　ii. 构造并初始化对象
其中，分配内存的动作由`operator new`函数完成，构造并初始化对象则由构造函数完成。那么，我们可以得出这样的结论：new expression其实是通过调用`operator new`函数和相应类型的构造函数完成动作的，更形象的可以描述为：

```C++
new expression(T) {		// 只是用于形象的描述
    T* t = operator new(sizeof(T));
    t->consturctor_of_T();
}
```

### 1.2 operator new
首先，描述下operator new函数是如何定义的：

```C++
replaceable allocation functions
1. void* operator new (std::size_t count);
2. void* operator new (std::size_t count, const std::nothrow_t& tag);

placement allocation functions
3. void* operator new (std::size_t count, void* ptr);
4. void* operator new(std::size_t count, user_defined_args...);

class-specific allocation functions
5. void* T::operator new(std::size_t count);
6. void* T::operator new(std::size_t count, user_defined_args...);
```


