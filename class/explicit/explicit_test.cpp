#include <cstdio>

/*
 * 1. only declarations of constructors can be ‘explicit’
 * 2. 类中无public/private/protected修饰的成员, 其属性默认是private
 *  在C++中，explicit关键字用来修饰类的构造函数，被修饰的构造函数的类，不能发生相应的隐式类型转换，只能以显示的方式进行类型转换
 */
class A
{
public:
    enum DAY{DAY_MON, DAY_TUE, DAY_WED, DAY_THU, DAY_FRI, DAY_SAT, DAY_SUN};
    explicit A(enum DAY day) {
        printf("day:%d\n", day);
    }

    A(int day) {
        printf("week:%d\n", day);
    }
};

int main(int argc, char **argv)
{
    printf("explicit test ...\n");
    
    A a(A::DAY_MON);
    A b(2);

    return 0;
}
