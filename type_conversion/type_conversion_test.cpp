/*
 * C++类型转换分为：隐式类型转换和显式类型转换
 * 隐式类型转换(又称为“标准转换”)
 * 1) 算术转换(Arithmetic conversion) : 在混合类型的算术表达式中, 最宽的数据类型成为目标转换类型。
 * 2)一种类型表达式赋值给另一种类型的对象：目标类型是被赋值对象的类型.
 *   例外：void指针赋值给其他指定类型指针时，不存在标准转换，编译出错
 * 3)将一个表达式作为实参传递给函数调用，此时形参和实参类型不一致：目标转换类型为形参的类型
 * 4)从一个函数返回一个表达式，表达式类型与返回类型不一致：目标转换类型为函数的返回类型
 * 显式类型转换(又称为“强制类型转换”)
 * 标准C++中有四个类型转换符：static_cast、dynamic_cast、reinterpret_cast、和const_cast
 *
 * Ref: http://www.cnblogs.com/chio/archive/2007/07/18/822389.html
 */
#include <stdio.h>
#include <typeinfo> // typeid()

static void arithmetic_conversion_test();
static void type_assignment_conversion_test();
int main(int argc, char **argv)
{
    printf("C++ type conversion test\n");
    arithmetic_conversion_test();
    type_assignment_conversion_test();

    return 0;
}

static void arithmetic_conversion_test()
{
	int radius = 7;
	double pi = 3.14;

	printf("perimeter = %f\n", 2 * pi * radius);
	if (typeid(double) != typeid(2 * pi * radius)) {
		printf("int not convert to double here\n");
	} else {
		printf("int Yes convert to double here\n");
	}
}

static void type_assignment_conversion_test()
{
	int week = 7;
	double day = week;
	printf("day = %f\n", day);
	// int *ptemp = (void *)2;
}
