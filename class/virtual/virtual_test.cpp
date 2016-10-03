/*
 * 1. 虚函数的定义: virtual type method_name() {可有语句, 也可无}
 * 2. 纯虚函数的定义: virtual type  method_name() = 0
 * 3. virtual修饰析构函数的作用: 当用一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用
 * 4. 当类里面有虚函数的时候，编译器会给类添加一个虚函数表，里面来存放虚函数指针，这样就会增加类的存储空间。
 *    所以，只有当一个类被用来作为基类的时候，才把析构函数写成虚函数。
 * 5. 在虚函数和纯虚函数的定义中不能有static标识符，因为被static修饰的函数在编译时候要求前期bind,然而虚函数却是动态绑定.
 *    cannot be declared both virtual and static
 * 6. 含有纯虚函数(个数>0)的类被称为抽象类, 抽象类不能被直接实例化, 只能被子类继承
 * 7. 在构造/析构函数中调用虚函数
 */
#include <iostream>
using namespace std;

static void virtual_destructor_test();
static void constructor_destructor_invoke_virtual_function_test();

int main(int argc, char **argv)
{
    virtual_destructor_test();
    constructor_destructor_invoke_virtual_function_test();
    
    return 0;
}

class Mother
{
public:
    Mother() {};
    virtual ~Mother() { cout << __func__ << endl; }

    virtual void DoSomething() { cout << "Ma Ma De Wen Shi TianMi De Wen" << endl; };
};

class Daughter : public Mother
{
public:
    Daughter() {};
    ~Daughter() { cout << "Zai Huan Ma Ma Yi Ge Wen" << endl; }; 

    void DoSomething() { cout << "Nv Er De Wen Chun Jie De Wen" << endl; };
};

static void virtual_destructor_test()
{
    cout << __func__ << " " << "test..." << endl;
    Mother *liuHuLan = new Daughter;
    liuHuLan->DoSomething();
    delete liuHuLan;
}

class Book {
public:
    Book() {
        VirtualReading();
    }
    
    virtual ~Book() {
        cout << "Book::" << __func__ << endl;
    }
    
    virtual int VirtualReading() { cout << "Book::" << __func__ << endl; return 0; }
    virtual int PureVirtualWritting() = 0;
};

class TextBook : public Book {
public:
    TextBook() {
        VirtualReading();
    }
    
    ~TextBook() {
        cout << "Book::" << __func__ << endl;
        PureVirtualWritting();
    }
    int VirtualReading() { cout << "TextBook::" << __func__ << endl; return 0; }
    int PureVirtualWritting() { cout << "TextBook::" << __func__ << endl; return 0; }
};

/*
 * 虚函数可以在构造函数和析构函数中调用，但虚函数此时是静态绑定；而非动态绑定。
 * 继承类在构造的时候总是首先调用其基类的构造函数来对属于其基类的部分进行构造，在这个时候，
 * 整个类被当作基类来处理，继承类的部分对整个类来说好像不存在一样，
 * 直到基类的构造函数退出并进入继承类的构造函数，该类才被当作继承类来出来处理。
 * 对析构也一样，只是析构的顺序正好相反。
 */
static void constructor_destructor_invoke_virtual_function_test()
{
    cout << __func__ << " " << "test..." << endl;
    TextBook *pTextBook =new TextBook;
    Book *pBook = pTextBook;
    pBook->VirtualReading();
    pBook->PureVirtualWritting();
    delete pBook; /*  polymorphic deletion */
}
