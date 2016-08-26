#include <stdio.h>

typedef void (*pFunction)();

static void function_address_test();
static void const_pointer_test();
int main(int argc, char **argv)
{
    function_address_test();
    const_pointer_test();
    printf("const_pointer_test = %p\n", const_pointer_test);
    return 0;
}

static void function() /* ‘void (*)()’ */
{
    printf("%s():%d\n", __FUNCTION__, __LINE__);
}

/*
 * 函数指针, 相当于常量指针, 它的值是函数所在代码的首地址, 不支持指针算术运算
 * 支持取地址运算, 解引用运算, 解引用的结果和取地址的结果一样. 仍然是函数所在代码的首地址.
 */
static void function_address_test()
{
    pFunction pFun = function;
    
    if (function == &function
     && function == pFun
     && pFun == *pFun
     && *pFun == **pFun
     && **pFun == ***pFun) {
        printf("function = %p\n", function);
        printf("&function = %p\n", &function);
        //printf("&&function = %p\n", &&function); 
        //error: lvalue required as unary ‘&’ operand
        printf("pFun = %p\n", pFun);
        //printf("pFun + 1 = %p\n", pFun + 1);
        printf("*pFun = %p\n", *pFun);
        printf("**pFun = %p\n", **pFun);
        printf("***pFun = %p\n", ***pFun);
    }
}

/*
 * label标号可以用&&运算符取其值.
 */
static void const_pointer_test()
{
__const_pointer_test:
    unsigned long *q = NULL;


    printf("&q = %p\n", &q);
    q = (unsigned long *)&q;
    printf("q = %p\n", q);
    printf("q + 1 = %p\n", q + 1);
    printf("*q = 0x%lx\n", *q);
    printf("**q = 0x%lx\n", **(unsigned long **)*q);
    //printf("b = %p\n", b); //error: ‘b’ was not declared in this scope
    printf("__const_pointer_test = %p\n", const_pointer_test);
    printf("&&__const_pointer_test = %p\n", &&__const_pointer_test);
}