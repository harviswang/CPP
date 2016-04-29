#include <iostream>

//template <typename T> /* ; */

/*
 * 模板函数: 交换两个元素的值
 */
template <typename T>
static void swap(T& a, T& b) {
	T tmp = a;
	a = b;
	b = tmp;
}

class Test {
public:
	void A(void);
public:
	int a;
};

void Test::A(void)
{
	std::cout << "call A() of class Test" << std::endl;
}

int main(int argc, char **argv)
{
	Test test;
	int a = 8;
	int b = 90;
	const int *arr[2] = {&a, &b};
	int& ra = a;
	int& rb = b;
	
	
	std::cout << "a = " << a << " " << "b = " << b << std::endl;
	swap(a, b);
	std::cout << "a = " << a << " " << "b = " << b << std::endl;
	test.A();
	return 0;
}