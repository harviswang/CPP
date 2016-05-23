#include <iostream>

/*
 * 初始化与赋值的含义：
 * 初始化：包括创建（或说定义）对象并且赋给初值。（如果一个对象只被创建而没有被初始化，则该变量只能用于被赋值）。
 * 赋值： 擦除对象的当前值并用新值代替。
 */
using namespace std;

static void class_type_copy_initialize_test();
static void class_type_direct_initialize_test();
static void basic_type_copy_initialize_test();
static void basic_type_direct_initialize_test();
static void basic_type_initialize_test();
static void class_type_initialize_test();
int main(int argc, char **argv)
{
    basic_type_initialize_test();
    class_type_initialize_test();
    return 0;
}

/*
 * 内置类型(基本类型)
 * (一) 直接初始化
 * 1. 空初始化: 即无参数无括号形式
 *            如int i，new int,new int[10].当在所有函数之外时，初始化为0；当在某一函数中时，没初始化。
 * 2. 值初始化: 即无参数有括号,且括号只能在类型名后, 而不能在变量名之后, 即只能创建无名对象.
 * 3. 显式初始化: 即有参数有括号形式, 且当未有名对象时括号在对象名之后, 为无名对象时括号在类/类型名之后.
 *
 * (二) 复制初始化
 * 1. 形式1
 *    如 int i = 5;
 * 2. 形式2
 *    如 int i1 = 5;
 *       int i2(i1)或int i2 = i1;
 * 3. 形式3
 *    如 int i(int(5)) 或 int i = int(5);
 */
static void basic_type_initialize_test()
{
    basic_type_direct_initialize_test();
    basic_type_copy_initialize_test();
}

/*
 * 变量i只创建未赋值, 并且它在函数内部,未赋值就使用是报警告的,打印出来值0,怎么不是随即值,奇怪!
 */
static void basic_type_direct_initialize_test()
{
    //空初始化
    {
        int i;
        new int;
        new int[10];

        (void)i;
        //cout << "i = " << i << endl;//variable 'i' is uninitialized when used here
    }

    //值初始化
    {
        //int(); //创建了一个无名对象，其被值初始化为0.一般将该无名对象初始化化或赋值给某有名对象,或直接作为无名对象使用
        cout << "int() = " << int() << endl;
        //new int();
        cout << "new int() = " << new int() << endl;
        new int[10]();
        cout << "new int[10]() = " << new int[10]() << endl;
    }

    //显式初始化
    {
        int i(5);
        cout << "i(5) = " << i << endl;
        new int(5);
        cout << "*(new int(5)) = " << *(new int(5)) << endl;
    }
}

static void basic_type_copy_initialize_test()
{
    //形式1
    {
        int i = 5;
        (void)i;
    }

    //形式2
    {
        int ia = 5;
        int ib(ia);
        int ic = ia;
        cout << "int ia = 5; int ib(ia); => ib = " << ib << endl;
        cout << "int ia = 5; int ic = ia; => ic = " << ic << endl;
    }

    //形式3
    {
        int i(int(5));
        cout << "int i(int(5)); => i = " << i << endl;
        int k = int(20160523);
        cout << "int k = int(20160523); => k = " << k << endl;
    }
}

class Cup {
public:
    Cup(){
        cout << "func:" << __func__ << " line:" << __LINE__ << endl;
    }

    /*
     * 拷贝构造函数
     * 参数为const修饰的
     */
    Cup(const Cup &cup) {
        //do nothing;
    }

    Cup(int color) {
        mColor = color;
        mPrice = 0;
    }

    Cup(int color, int price) {
        mColor = color;
        mPrice = price;
    }
    /*
     * 析构函数可以被显式调用
     */
    ~Cup() {
        cout << "func:" << __func__ << " line:" << __LINE__ << endl;
    }

private:
    int mColor;
    int mPrice;
};

/*
 * 类类型
 * (一) 直接初始化
 * 1. 空初始化: 即无参数无括号形式
 *             如Cup obj;或new Cup;或new Cup[2];当class Cup有默认构造函数时, 用改默认构造函数初始化;
 *             若class Cup没有默认构造函数时, 则不能这样定义(error: no matching constructor for initialization of 'Cup').
 * 2. 值初始化: 即无参数有括号形式, 且括号只能在类型名后, 而不能在变量名之后, 即只能创建无名对象或者在类的初始化列表中.
 *             若有默认构造函数, 则用这个默认构造函数初始化.
 *             若无默认构造函数, 则编译报错(error: no matching function for call to ‘Cup::Cup()’)
 *             因为有括号没参数, 就是函数调用, 就需要函数的参数返回值的强匹配, 匹配不上就报错.
 *             括号只能写在最后, 不然就不是无参数函数调用了, 就报错(error: expected ‘;’ before ‘[’ token)
 * 3. 显式初始化: 即有参数有括号形式, 且当为有名对象时括号在对象名之后, 为无名对象时括号在类类型名之后
 *              如Cup obj(1, 2); 或 new Cup(1, 2);
 * (二) 复制初始化:
 * 1. 形式1
 *    如Cup cup = 9;类Cup中有一个非explicit的只有一个型参或其引用的构造函数. 构造函数参数不能用默认值.
 * 2. 形式2
 *    如Cup cup(9);
 * 3. 形式3
 *    如Cup cup(Cup(2, 3));或Cup cup = Cup(2, 3);
 * 注意1: 在初始化式中符号'='不是赋值操作符, 它时拷贝构造函数;
 * 注意2: 拷贝构造函数就是指仅有一个型参且该型参时本身类类型的const引用的构造函数;
 * 注意3：const对象的初始化同上，只是初始化后其值不能再更改；
 * 注意4：引用类型对象的初始化：若不初始化，则其没有意义；
 *       非const引用只能指向 同类型的 非const的 对象
 *       const引用能指向 相关类型的 const或非const的 对象或字面值，但不可以通过该引用去修改其绑定的对象。
 */
static void class_type_initialize_test()
{
    class_type_direct_initialize_test();
    class_type_copy_initialize_test();
}

static void class_type_direct_initialize_test()
{
    //空初始化
    cout << "空初始化" << endl;
    {
        Cup obj;
        delete new Cup;
        delete[] new Cup[0]; //不会调用构造/析构函数的
    }

    //值初始化
    cout << "值初始化" << endl;
    {
        Cup(); //创建了一个无名对象，一般将该无名对象初始化化或赋值给某有名对象,或直接作为无名对象使用
        delete new Cup();
        //cout << "new Cup() = " << new Cup() << endl;
        delete[] new Cup[2]();
    }

    //显式初始化
    cout << "显式初始化" << endl;
    {
        Cup paper_cup(1, 2);
    }
}

static void class_type_copy_initialize_test()
{
    //形式1
    cout << "形式1" << endl;
    {
        Cup cup = 9;
    }

    //形式2
    cout << "形式2" << endl;
    {
        Cup cup(99);
        Cup paper_cup = 88;
        Cup blue_cup(paper_cup);
        Cup glass_cup = Cup(101);
    }

    //形式3
    cout << "形式3" << endl;
    {
        Cup cup(Cup(2, 3));
        Cup c = Cup(2, 3);
    }

}
