/*
 * A class declared inside a function becomes local to that function and is 
 * called Local Class in C++.
 * Following are some interesting facts about local classes.
 * 1) A local class type name can only be used in the enclosing function.
 * 2) All the methods of Local classes must be defined inside the class only.
 *    函数内部不允许定义函数/方法
 * 3) A Local class cannot contain static data members. 
 *    It may contain static functions though.  
 * 4) Member methods of local class can only access static and enum variables of
 *    the enclosing function. Non-static variables of the enclosing function are
 *    not accessible inside local classes.
 * 5) Local classes can access global types, variables and functions. 
 *    Also, local classes can access other local classes of same function
 * 
 */
#include <stdio.h>

static void local_class_demo();
static void local_class_scope_test();

int main(int argc, char *argv[])
{
	printf("local class test...\n");
	local_class_demo();
	local_class_scope_test();

	return 0;
}

static void local_class_demo()
{
	enum { ONE=1, TWO=2, THREE=3};
	static int ott = 99;
	class Test {
	public:
		static void method ()
		{
			printf("class: Test, method: %s\n", __func__);
			printf("ott = %d\n", ott);
		}
		void out();
	private:
		//static int content;
	};
	//void Test::out() {}
	
	Test test;
	printf("&test = %p\n", &test);
	//test.method();
	
	class Potato {
	public:
		static void about() 
		{ 
			Test::method();
			printf("Potato delicious\n"); 
		}
	};
	
	Potato::about();
}

static void local_class_scope_test()
{
	//Test t;
}