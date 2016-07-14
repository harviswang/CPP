/*
 * 1. 虚函数的定义: virtual method_name() {可有语句, 也可无}
 * 2. 纯虚函数的定义: virtual method_name() = 0
 * 3. virtual修饰析构函数的作用: 当用一个基类的指针删除一个派生类的对象时，派生类的析构函数会被调用
 * 4. 当类里面有虚函数的时候，编译器会给类添加一个虚函数表，里面来存放虚函数指针，这样就会增加类的存储空间。
 *    所以，只有当一个类被用来作为基类的时候，才把析构函数写成虚函数。
 * 5. 在虚函数和纯虚函数的定义中不能有static标识符，因为被static修饰的函数在编译时候要求前期bind,然而虚函数却是动态绑定.
 *    cannot be declared both virtual and static
 * 6. 含有纯虚函数(个数>0)的类被称为抽象类, 抽象类不能被直接实例化, 智能被子类继承
 */
#include <iostream>
using namespace std;

class Mother
{
public:
    Mother() {};
    virtual ~Mother() { }

    virtual void DoSomething() { cout << "Ma Ma De Wen Shi TianMi De Wen" << endl; };
};

class Daughter : public Mother
{
public:
    Daughter() {};
    ~Daughter() { cout << "Zai Huan Ma Ma Yi Ge Wen" << endl; }; 

    void DoSomething() { cout << "Nv Er De Wen Chun Jie De Wen" << endl; };
};

int main(int argc, char **argv)
{
    Mother *liuHuLan = new Daughter;
    liuHuLan->DoSomething();
    delete liuHuLan;

    return 0;
}