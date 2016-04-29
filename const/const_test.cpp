#include <iostream>
using namespace std;

int foo( int &x)
{
	int a = 88;
	return a;
}

void bar(const string & s)
{
    cout << s << endl;
}

int main(int argc, char **argv)
{
	int a = 99;
	cout << foo(foo(a));
	bar("hello");
	return 0;
}
