#include <stdio.h>

/*
 * 
 */
class CC { /* CC means Copy Constructor here */
public:
    CC(){ printf("%s() %d\n", __func__, __LINE__); }
    CC(CC &c) {
        //return c;
        printf("%s() %d\n", __func__, __LINE__);
        this->a = c.a;
    }
    ~CC(){ printf("%s() %d\n", __func__, __LINE__); }

public:
    int a;
};

/*
 * 对象以值传递的方式传入函数参数
 * 对象c传入后, 会生成一个临时对象temp_c, 
 * temp_c = c, 调用拷贝构造函数
 * 函数退出后, 析构temp_c临时对象
 */
static void g_func(CC c)
{
    printf(" c.a = %d\n", c.a);
}

static void object_as_lvalue_test()
{
    printf("%s() test...\n", __func__);
    /*
     * c,b运行了一次默认构造函数, 一次拷贝构造函数, 两次默认析构函数
     */
    CC c;
    CC b(c); /* CC b  = c;*/ /* 运行默认拷贝构造函数, 而非赋值 */
}

static void object_as_input_argument_test()
{
    printf("%s() test...\n", __func__);
    CC c;
    g_func(c);
}

static CC f_func()
{
    CC c;
    return c;
}

/*
 * 
 */
static void object_as_return_value_test()
{
    printf("%s() test...\n", __func__);
    /*CC b = */f_func(); /* CC b = f_func(); 是非法的 */
}

class Rect {
public:
    Rect(){
        count++;
        printf("%s() count=%d\n", __func__, count);
    }
    ~Rect() {
        count--;
        printf("%s() count=%d\n", __func__, count);
    }
    /* 非默认构造函数 */
    Rect(const Rect& r)
    {
        width = r.width;
        height = r.height;
        count++;
    }
    static int getCount() {
        return count;
    }

private:
    int width;
    int height;

public:
    static int count;
};
int Rect::count = 0;

class Mem
{
public:
    Mem() { 
        p = new int(100); /*  */
    }
    ~Mem() { 
        if (p)  {
            delete p;
        } 
    }
    /* 深拷贝构造函数 */
    Mem(const Mem& r) {
        p = new int;
        *p = *(r.p);
    }
private:
    int *p;
};

/*
 * 默认拷贝构造函数没有处理静态数据成员, 需要编写处理静态数据成员的拷贝构造函数
 */
static void shallow_copy_test()
{
    printf("%s() test...\n", __func__);
    Rect rect1;
    printf("The count of Rect: %d\n", Rect::getCount());
    
    Rect rect2(rect1);// 使用rect1复制出rect2，此时应该有两个对象, 使用的是默认拷贝构造函数
    printf("The count of Rect: %d\n", Rect::getCount());
    
    Mem m1;
    Mem m2(m1);
}

/*
 * 就类对象而言，相同类型的类对象是通过拷贝构造函数来完成整个复制过程的
 * 拷贝构造函数原型
 * CLASSNAME(const CLASSNAME& x, int b = 0, int c = 8, ...)
 * 函数的名称必须和类名称一致，它必须的一个参数是本类型的一个引用变量
 * 1. 对象以值传递的方式传入函数参数
 * 2. 对象以值传递的方式从函数返回
 * 3. 对象需要通过另外一个对象进行初始化
 * 4. 浅拷贝, 浅拷贝，指的是在对象复制时，只对对象中的数据成员进行简单的赋值，默认拷贝构造函数执行的也是浅拷贝。
 * 5. 深拷贝
 * 6. 防止默认拷贝发生, 声明司有默认拷贝构造函数
 */
void copy_constructor_test(void)
{   
    object_as_lvalue_test();
    object_as_input_argument_test();
    object_as_return_value_test();
    shallow_copy_test();
}