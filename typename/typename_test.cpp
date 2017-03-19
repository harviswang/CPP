/*
 * 1. typename关键字告诉了编译器把一个特殊的名字解释成一个类型
 * 2. 在声明一个 template type parameter（模板类型参数）的时候，class 和 typename 意味着完全相同的东西
 *    例如template<class T> class Widget; 和 template<template T> class Widget;等价.
 *    然而，C++ 并不总是把 class 和 typename 视为等同的东西。有时你必须使用 typename。为了理解这一点，我们不得不讨论你会在一个 template（模板）中涉及到的两种名字.
 */
#include <iostream>
#include <stdio.h>
#include <typeinfo> /* typeidf() */
#include <vector>

template <typename T>
class A { 
    public: 
    typedef T a_type; 
}; 

template <typename L>
class B { 
    public: 
    //typedef A::a_type b_type;
    typedef typename L::a_type b_type;
}; 

template <typename C>
void print2nd(const C& container)
{
    if (container.size() >= 2) {
        typename C::const_iterator iter(container.begin());
        ++iter;
        int value = *iter;
        printf("value = %d\n", value);
    }
}

int main()
{
    printf("typename keyword test ...\n");
    typedef A<int> a_int_type;
    typedef B<a_int_type> b_int_type;
    b_int_type::b_type i;
    a_int_type::a_type j;
    
    if (typeid(i) == typeid(j)) {
        printf("YES\n");
    }

	std::vector<int> v;
	v.push_back(0);
	v.push_back(1);
	v.push_back(2);
	print2nd(v);
    
    return 0;
}
