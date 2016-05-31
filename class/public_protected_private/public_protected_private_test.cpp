/*
 * 1. 类的一个特征就是封装，public和private作用就是实现这一目的。
 *    所以：用户代码（类外）可以访问public成员而不能访问private成员；
 *         private成员只能由类成员（类内）和友元访问。
 * 2. 类的另一个特征就是继承，protected的作用就是实现这一目的。
 *    所以：用户代码（类外）可以访问public成员而不能访问protected成员；
 *         protected成员可以被派生类对象访问。
 *
 * 继承中public/protected/private的特点：
 * 先记住：不管是否继承，上面的规则永远适用！
 * 有public, protected, private三种继承方式，它们相应地改变了基类成员的访问属性。
 * 1. public继承：
 *    基类public成员，protected成员，private成员的访问属性在派生类中分别变成：
 *       public,    protected,    private
 * 2. protected继承：
 *    基类public成员，protected成员，private成员的访问属性在派生类中分别变成：
 *       protected, protected,    private
 * 3. private继承：
 *    基类public成员，protected成员，private成员的访问属性在派生类中分别变成：
 *       private,   private,      private
 * 但无论哪种继承方式，上面两点都没有改变：
 * 1. private成员只能被本类成员（类内）和友元访问，不能被派生类访问；
 * 2. protected成员可以被派生类访问。
 * 3. public成员可以被派生类访问.
 * Ref: http://www.jb51.net/article/54224.htm
 */
#include <iostream>
#include <assert.h>
#include <cstdio>
using namespace std;

class A {
public:
    A() {
        a1 = 1;
        a2 = 2;
        a3 = 3;
        a = 4;
    }
    void fun() {
        cout << a << endl;
        cout << a1 << endl;
        cout << a2 << endl; //正确，类内访问protected成员
        cout << a3 << endl; //正确，类内访问private成员
    }
public:
    int a;
    int a1;
protected:
    int a2;
private:
    int a3;
};

class B : public A {
public:
    B(int i) {
        A();
        a = i;
    }
    void fun() {
        cout << a << endl;  //正确，public成员, 是B.a, 不是A.a
        cout << a1 << endl; //正确，基类的public成员，在派生类中仍是public成员。
        cout << a2 << endl; //正确，基类的protected成员，在派生类中仍是protected可以被派生类访问。
        //cout << a3 << endl; //错误，基类的private成员不能被派生类访问。
    }
public:
    int a; // 与父类重名, 调用以对象所属类为准
};

class C : protected A {
public:
    C(int i) {
        A();
        a = i;
    }
    void fun() {
        cout << a << endl;
        cout << a1 << endl;
        cout << a2 << endl;
        //cout << a3 << endl; //private成员只能被本类成员（类内）和友元访问，不能被派生类访问；
    }
public:
    int a;
};

class D : private A {
public:
    D(int i) {
        A();
        a = i;
    }
    void fun() {
        cout << a << endl;
        cout << a1 << endl;
        cout << a2 << endl;
        //cout << a3 << endl; //private成员只能被本类成员（类内）和友元访问，不能被派生类访问；
    }
public:
    int a;
};

static void A_test(void);
static void B_test(void);
static void C_test(void);
static void D_test(void);
static void E_test(void);
static void F_test(void);
static void G_test(void);
int main(int argc, char **argv)
{
    A_test();
    B_test();
    C_test();
    D_test();
    E_test();
    F_test();
    G_test();
    
    return 0;
}

static void A_test(void)
{
    A itema;
    cout << __func__ << "() test..." << endl;
    itema.a = 10;
    itema.a1 = 20;
//    itema.a2 = 30; //错误，类外不能访问protected成员
//    itema.a3 = 40; //错误，类外不能访问private成员
    itema.fun();
}
static void B_test(void)
{
    B b(10);
    cout << __func__ << "() test..." << endl;
    cout << b.a << endl;
    cout << b.a1 << endl;
//    cout << b.a2 << endl; //错误，类外不能访问protected成员
//    cout << b.a3 << endl; //错误，类外不能访问private成员
    b.fun();
}

static void C_test(void)
{
    C c(10);
    cout << __func__ << "() test..." << endl;
    cout << c.a << endl;
//    cout << c.a1 << endl; //错误，protected成员不能在类外访问。
//    cout << c.a2 << endl; //错误，类外不能访问protected成员
//    cout << c.a3 << endl; //错误，类外不能访问private成员
    c.fun();
}

static void D_test(void)
{
    D d(10);
    cout << __func__ << "() test..." << endl;
    cout << d.a << endl;
//    cout << d.a1 << endl; //错误，private成员不能在类外访问。
//    cout << d.a2 << endl; //错误，private成员不能在类外访问。
//    cout << d.a3 << endl; //错误，private成员不能在类外访问。
    d.fun();    
}

/*
 * 测试父类指向子类
 */
static void E_test(void)
{
    cout << __func__ << "() test..." << endl;
//    A *pa = new D(99); //error: cannot cast 'D' to its private base class 'A'
//    A *pa = new C(9);//error: cannot cast 'C' to its protected base class 'A'
    A *pa = new B(9);
    pa->fun();
    
//    B *pb = new A();//error: cannot initialize a variable of type 'B *' with an rvalue of type 'A *'
}

class E {
public:
    E() {
        a = 0;
    }
    
    int a;
};

class F : public E {
public:
    F() {
        a = 1;
    }
    
    int a;
};

/*
 * 1. 父类中的a和子类中的a是两个不同的变量。
 * 2. pf->a的地址是pf的值加上a的偏移量。
 * 3. 父类的属性一般比较靠前，父类中的a的偏移量为0，子类中的a的偏移量为4。
 * 4. 如果pe是父类的指针，pe->a的地址等于pe的值加上0。如果pf是子类的指针，pf->a的地址等于pf的值加上4。
 * 5. 子类并不能够覆盖父类的属性,而是追加在父类后面
 * 6. C++编译器都是通过属性类型长度和偏移量去定位它的属性的, java不是, java会在实例中保存类的坐标
 * 7. 通过对象指针进行的普通成员函数的调用，仅仅与指针的类型有关，而与此刻指针正指向什么对象无关。
 */
static void F_test(void)
{
    F *pf = new F();
    E *pe = pf;

    printf("%s () test...\n", __func__);
    
    printf("a in F is: %d\n", pf->a);
    printf("a in E is: %d\n", pe->a);
    
    printf("value of pf is: %p\n", pf);
    printf("value of pe is: %p\n", pe);
    
    printf("address of pf->a is: %p\n", &(pf->a));
    printf("address of pe->a is: %p\n", &(pe->a));
}

/*
 * 1. 派生类包含了基类所有成员以及新增的成员，同名的成员被隐藏起来，调用的时候只会调用派生类中的成员。
 * 2. 子类调用基类的同名成员
 */
static void G_test(void)
{
    printf("%s () test...\n", __func__);
    printf("sizeof(A) = %ld\n", sizeof(A));
    printf("sizeof(B) = %ld\n", sizeof(B));
    printf("sizeof(C) = %ld\n", sizeof(C));
    printf("sizeof(D) = %ld\n", sizeof(D));
    
    B b(88);
    printf("b.a = %d\n", b.a);
    printf("b.A::a = %d\n", b.A::a); //子类B调用基类A的同名成员a
    
}