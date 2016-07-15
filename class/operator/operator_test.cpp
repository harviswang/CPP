/*
 * operator是C++的关键字，它和运算符一起使用，表示一个运算符函数，理解时应将operator=整体上视为一个函数名。
 * 1. 操作符重载实现为类成员函数
 * 2. 操作符重载实现为非类成员函数(全局函数)
 * 3. 一般情况下，单目运算符最好重载为类的成员函数；双目运算符则最好重载为类的友元函数
 * 
 * 重载操作符具有以下限制：
 * 1. 只有C++预定义的操作符集中的操作符才可以被重载
 *     C++中可以重载的运算符: ==, !=, +=, -=, <, >, <=, >=, =, (), [], ++, --, *, ->
 * 2. 对于内置类型的操作符，它的预定义不能被改变，应不能为内置类型重载操作符，如，不能改变指针类型的操作符<的含义
 * 3. 也不能为内置的数据类型定义其它的操作符
 * 4. 只能重载类类型或枚举类型的操作符
 * 5. 重载操作符不能改变它们的操作符优先级
 * 6. 重载操作符不能改变操作数的个数
 * 7. 除了对( )操作符外，对其他重载操作符提供缺省实参都是非法的
 * 
 * 注意点:
 * 1. 重载操操作符先确定返回值类型. 类型是左值 ? 返回引用 : 返回值
 * 2. 重载操操作符最高效率是返回引用或指针, 次高效率是返回基本数据类型, 比如int, 同时提供型参int类型的构造函数
 * 3. 在增量运算符中，放上一个整数形参，就是后增量运行符，它是值返回，对于前增量没有形参，而且是引用返回
 * 4. 一般来说，转换运算符与转换构造函数（即带一个参数的构造函数）是互逆的
 * 5. 以下一些双目运算符不能重载为类的友元函数：=、()、[]、->
 */
#include <iostream>
#include <stdio.h>

class Person
{
#define DEBUG() printf("%s():%d\n", __func__, __LINE__)
public:
    Person() { mAge = 0; }
    Person(int age) { DEBUG(); mAge = age; }
 
    // Assignment
    Person(const Person& o) { DEBUG(); }
    Person(const Person* o) { DEBUG(); }
    
    // compare operator
#define COMPARE(_op_)                    \
bool operator _op_ (const Person& o) {   \
    DEBUG();                             \
    return mAge _op_ o.mAge;             \
}                                        \
bool operator _op_ (const Person* o) {   \
    DEBUG();                             \
    return mAge _op_ o->mAge;            \
} 
    COMPARE(==);
    COMPARE(!=);
    COMPARE(>);
    COMPARE(<);
    COMPARE(>=);
    COMPARE(<=);
#undef COMPARE
    // conversion operator
    operator Person ();
    operator int () { DEBUG(); return mAge; }
    
    // i++/++i operator
    Person& operator ++ () { DEBUG(); mAge++; return *this; }
    Person operator ++ (int) { DEBUG(); Person tmp(*this); tmp.mAge++; return tmp; }
    Person& operator += (int age) { DEBUG(); mAge += age; return *this; }
    Person& operator -= (int age) { return operator+=( -age ); }
    
    void printAge() { DEBUG(); printf("mAge = %d\n", mAge); }

private:
    int mAge;
#undef DEBUG
};

int main(int argc, char **arv)
{
    printf("operator test ...\n");
    Person xiaoMing = Person(10);
    Person zhangSan = Person(11);
    
#define TEST(_op_) printf(#_op_" %d\n", xiaoMing _op_ zhangSan);
    TEST(==);
    TEST(!=);
    TEST(>);
    TEST(<);
    TEST(>=);
    TEST(<=);
#undef TEST

    (Person)xiaoMing;
    ++xiaoMing;
    xiaoMing.printAge();
    xiaoMing++;
    xiaoMing.printAge();
    
    int a = xiaoMing;
    printf("a = %d\n", a);
    int b = zhangSan;
    printf("b = %d\n", b);
    
    Person liSi = a; // a+b -> int(c);;; Person(c) -> liSi
    liSi += b;
    liSi.printAge();
    liSi -= b;
    liSi.printAge();
    
    return 0;
}