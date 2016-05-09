#include <iostream>
#include <string>
using namespace std;

static void reference_synopsis(void);
static void reference_as_argument(void);
static void reference_const(void);
static void reference_as_return(void);

int main(int argc, char **argv)
{
    reference_synopsis();
    reference_as_argument();
    reference_const();
    reference_as_return();
	return 0;
}

/*
 * 测试引用
 * 引用就是某一变量（目标）的一个别名，对引用的操作与对变量直接操作完全一样。
 * 引用,本质是别名,它不占内存
 * 引用的声明方法：类型标识符 &引用名=目标变量名；
 * int a; int &ra = a;
 * 1. &在此不是求地址运算，而是起标识作用。
 * 2. 类型标识符是指目标变量的类型。
 * 3. 声明引用时，必须同时对其进行初始化。
 * 4. 引用声明完毕后，相当于目标变量名有两个名称，即该目标原名称和引用名，且不能再把该引用名作为其他变量名的别名。
 * 5. 声明一个引用，不是新定义了一个变量，它只表示该引用名是目标变量名的一个别名，它本身不是一种数据类型，
 *    因此引用本身不占存储单元，系统也不给引用分配存储单元。故：对引用求地址，就是对目标变量求地址。&ra与&a相等。
 * 6. 不能建立数组的引用。因为数组是一个由若干个元素所组成的集合，所以无法建立一个数组的别名。
 */
static void reference_synopsis()
{
    int a = 2;
    int &ra = a;
//    int b[] = {1, 2};
//    int &rb[0] = {b[0]};

    if (&a == &ra) {
        cout << "the address of the variable and it's reference are the same" << endl;
        cout << "so they are one thing with two different name" << endl;
    }

}

static void int_swap(int &p1, int &p2)
{
    int p = p1;
    p1 = p2;
    p2 = p;

    cout << "&p1 = " << &p1 << endl;
    cout << "&p2 = " << &p2 << endl;
}

/*
 * 引用作为函数参数使用
 * 1. 传递引用给函数与传递指针的效果是一样的。这时，被调函数的形参就成为原来主调函数中的实参变量或对象的一个别名来使用，
 *    所以在被调函数中对形参变量的操作就是对其相应的目标对象（在主调函数中）的操作。
 * 2. 使用引用传递函数的参数，在内存中并没有产生实参的副本，它是直接对实参操作；而使用一般变量传递函数的参数，当发生函数调用时，
 *    需要给 形参分配存储单元，形参变量是实参变量的副本；如果传递的是对象，还将调用拷贝构造函数。因此，当参数传递的数据较大时，
 *    用引用比用一般变量传递参数的效 率和所占空间都好。
 * 3. 使用指针作为函数的参数虽然也能达到与使用引用的效果，但是，在被调函数中同样要给形参分配存储单元，且需要重复使用"*指针变量名"的
 *    形式进行运算，这很容易产生错误且程序的阅读性较差；另一方面，在主调函数的调用点处，必须用变量的地址作为实参。而引用更容易使用，更清晰。
 */
static void reference_as_argument(void)
{
    int a = 99;
    int b = 22;
    
    cout << "&a = " << &a << endl;
    cout << "&b = " << &b << endl;

    cout << "a = " << a << " " << "b = " << b << endl;
    int_swap(a, b);
    cout << "a = " << a << " " << "b = " << b << endl;
}

static void reference_const_helper(void)
{
    int a;
    const int &ra = a; // read-only reference

    //ra = 1; // error
    (int &)ra = 1; // 'const int &' convert to 'int &' then it's writeable
    //a = 8;

    cout << "a =" << a << endl;
}

string foo()
{
    return __FUNCTION__;
}

void bar(const string &s)
{
    cout << s << endl;
}

/*
 * 引用作为函数参数,
 * 0. 常引用声明方式：const 类型标识符 &引用名=目标变量名；
 * 1. foo( )和"hello"串都会产生一个临时对象，而在C++中，这些临时对象都是const类型的。
 *    试图将一个const类型的对象转换为非const类型，这是非法的.
 * 2. 引用作为函数参数, 调用函数必须传递l-value
 * 3. 引用型参数应该在能被定义为const的情况下，尽量定义为const
 * 4. 如果既要利用引用提高程序的效率，又要保护传递给函数的数据不在函数中被改变，就应使用常引用。
 * 5.
 */
static void reference_const(void)
{
    reference_const_helper();
    bar(foo());
    bar("hello");
}

/*
 * 函数返回临时变量的引用
 * 1. 临时变量的引用
 */
int &reference_as_return_temp_ref(void)
{
    static int a = 99; // if no static it's warning
    //int a = 99;
    return a;
}

float circle_area(float r)
{
    return (float)(3.14 * r * r);
}

float temp;
float &circle_area2(float r)
{
    temp = (float)(3.14 * r * r);
    return temp;
}
/*
 * 引用作为返回值
 * 类型标识符 &函数名（形参列表及类型说明）
 * {函数体}
 * 1. 以引用返回函数值，定义函数时需要在函数名前加&
 * 2. 用引用返回一个函数值的最大好处是，在内存中不产生被返回值的副本
 * 注意:
 * 1. 不能返回局部变量的引用
 * 2. 不能返回函数内部new分配的内存的引用
 * 3. 可以返回类成员的引用，但最好是const
 * 4. 引用与一些操作符的重载, 例如 <<, >>
 * 5. 在一些操作符中，却千万不能返回引用：+ - * / 四则运算符
 */
static void reference_as_return(void)
{
    int b = reference_as_return_temp_ref();
    cout << b << endl;

    float radius = 10.0;
    float area = circle_area((float)b);
    cout << "area is " << area << " with radium " << b << endl;

    float area2 = circle_area2(radius);
    cout << "area2 is " << area2 << " with radium " << radius << endl;
    cout << "temp is " << temp << endl;

//    float &area3 = circle_area(radius); // circle_area(radius) is not l-value
//    cout << "area3 is " << area3 << " with radium " << radius << endl;

    float &area4 = circle_area2(radius);
    cout << "area4 is " << area4 << " with radium " << radius << endl;

    circle_area2(radius) = 789.0;
    cout << "temp is " << temp << endl;
}
