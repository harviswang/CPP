#include <iostream>

#define SYMBOL2STRING(s) (#s)

/*template <class T>*/ // OK
template <typename T> // OK
void swap(T &a, T &b)
{
	T temp;
	temp = a;
	a = b;
	b = temp;
}

#define swap_test(a, b)                                                                          \
do {                                                                                                            \
	std::cout << SYMBOL2STRING(a) << " = " << a << std::endl;      \
	std::cout << SYMBOL2STRING(b) << " = " << b << std::endl;     \
	swap(a, b);                                                                                        \
	std::cout << SYMBOL2STRING(a) << " = " << a << std::endl;      \
	std::cout << SYMBOL2STRING(b) << " = " << b << std::endl;     \
} while (0)

static void template_function_test();
static void template_class_test();
int main(int argc, char **argv)
{
	template_function_test();
	template_class_test();

	return 0;
}

static void template_function_test()
{
	int a = 2016;
	int b = 1998;
	swap_test(a, b);

	double p = 3.14;
	double q = 1998.2016;
	swap_test(p, q);

	float c = 0.99;
	float d = 99.00;
	swap_test(c, d);

	/*swap(2010, 2020);*///no matching argument
}

template <class T>
class Test {
public:
	Test(){}
	~Test(){}
	void swap() {
			T temp = this->a;
			this->a = this->b;
			this->b = temp;
	}

	void setA(const T a) { this->a = a;}
	void setB(const T b) { this->b = b;}
	const T& getA() { return this->a;}
	const T& getB() { return this->b;}
	Test add(Test &a, Test &b) { 
		Test temp;
		temp.setA( add(a.getA(), b.getA()));
		temp.setB( add(a.getB(), b.getB()));
		return temp;
	}

	const T add(const T &a, const T &b) { return a+b;	}

	void dump() {
		std::cout << this->a << std::endl;
		std::cout << this->b << std::endl;
	}
private:
	T a;
	T b;
};
static void template_class_test()
{
	Test<int> a, b, c;
	a.setA(9);
	a.setB(8);
	a.dump();
	a.swap();
	a.dump();

	b.setA(1);
	b.setB(2);
	
	c = a.add(a, b);
	c.dump();
}
