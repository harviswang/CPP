/*
 * 1. typename关键字告诉了编译器把一个特殊的名字解释成一个类型
 * 2. 在声明一个 template type parameter（模板类型参数）的时候，class 和 typename 意味着完全相同的东西
 *    例如template<class T> class Widget; 和 template<template T> class Widget;等价.
 *    然而，C++ 并不总是把 class 和 typename 视为等同的东西。有时你必须使用 typename。为了理解这一点，我们不得不讨论你会在一个 template（模板）中涉及到的两种名字.
 */
#include <iostream>  
   
template <typename T>  
struct check  
{  
    check(void)  { std::cout << typeid(T).name(); }  
    ~check(void) { std::cout << std::endl; }  
};  
   
#define CHECK_TYPE__(OPT) \
    template <typename T> \
    struct check<T OPT> : check<T> \
    { \
        check(void) { std::cout << " "#OPT; } \
    };
   
CHECK_TYPE__(const)  
CHECK_TYPE__(volatile)  
CHECK_TYPE__(const volatile)  
CHECK_TYPE__(&)  
CHECK_TYPE__(&&)  
CHECK_TYPE__(*)  
   
int main(void)  
{  
    check<const volatile void * const*&>();  
    return 0;  
}
