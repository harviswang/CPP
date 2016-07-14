#include <cstdio>

/*
 * 1. only declarations of constructors can be ‘explicit’
 * 2. 类中无public/private/protected修饰的成员, 其属性默认是private
 */
class A
{
public:
    enum DAY{DAY_MON, DAY_TUE, DAY_WED, DAY_THU, DAY_FRI, DAY_SAT, DAY_SUN};
    explicit A(enum DAY day) {
        printf("day:%d\n", day);
    }
};

int main(int argc, char **argv)
{
    printf("explicit test ...\n");
    
    A a(A::DAY_MON);
    return 0;
}