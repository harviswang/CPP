#include <new>
#include <iostream>

static void normal_single_new_test(void);
static void normal_array_new_test(void);
static void placement_single_new_test(void);
static void placement_array_new_test(void);

int main(int argc, char *argv[])
{
    normal_single_new_test();
    normal_array_new_test();
    placement_single_new_test();
    placement_array_new_test();

    return 0;
}

static void normal_single_new_test()
{
    
}

static void normal_array_new_test()
{
    struct A {

        A(int ia, int ib) {
            a = ia;
            b = ib;
        }

        int a;
        int b;
    };
    int *buf = new int[128];
    A *pa = new(buf) A(3, 5);

    std::cout << "pa=" << pa << std::endl;
    std::cout << "buf=" << buf << std::endl;
    delete[] buf;
}

static void placement_single_new_test()
{

}

static void placement_array_new_test()
{
    
}