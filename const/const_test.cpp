#include <iostream>
using namespace std;

static void const_use_in_argument_test();
static void const_use_in_return_test();
static void const_array_storage_test();
int main(int argc, char **argv)
{
	const_use_in_argument_test();
	const_use_in_return_test();
	const_array_storage_test();

	return 0;
}

static int foo(/*const */int *x)
{
	int a = 88;
	return a;
}

static void bar(const string & s)
{
    cout << s << endl;
}
//
static void const_use_in_argument_test()
{
	const int a = 99;
	int b = 0;

	cout << foo((int *)&a) << endl;
	cout << foo(&b) << endl;
	bar("hello");

	/*a = b;*/ // Error
	b = a; // OK
}

static void const_use_in_return_test()
{
class A{
public:
	A():num(2){}
	~A(){}

	/*
	 * const here means (const this)
	 */
	void setNum(int num) const {this->num = num;}
	int getNum() { return this->num;}
	const int getNum() const { return this->num << 2;}
private:
	mutable int num; // can be modified in const context
};

class B {
public:
	B(){}
	~B(){}

	const A* get() { return new A(); }
};

	A a;
	a.setNum(100);
	int x = a.getNum();
	const A a2;
	a2.setNum(100);
	const int y = a2.getNum();
	std::cout << "x:" << x << "y:" << y << std::endl;
	
	/*
	 * const object can only access it's const function
	 * non-const object can access it's both const and non-const function
	 */	
	B b;
	b.get()->getNum();
	((A *)b.get())->setNum(99);
	((A *)b.get())->getNum();
	delete b.get();
}

/*
 * non const array used more storage than const array
 */
static void const_array_storage_test()
{
	/*const*/ int array[100] = {0};
	cout << "sizeof(array) = " << sizeof(array) << endl;
}
