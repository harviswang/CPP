#include <iostream>
#include "nullptr.h"
// nullptr/NULL -std=c++0x

void foo(int i)
{
	std::cout << "void foo(int i)" << std::endl;
}

void foo(char *p)
{
	std::cout << "void foo(char *p)" << std::endl;
}

int main(int argc, char *argv[])
{
	//std::cout << "NULL:" << NULL << std::endl;
	//std::cout << "nullptr:" << nullptr << std::endl;
	foo(0);
	foo(nullptr_user);

	return 0;
}
