#include <stdio.h>
#include <iostream>

class nullptr_t_t 
{
public:
	template<class T>          operator T*() const { return 0; }
	template<class C, class T> operator T C::*() const { return 0; }
private:
	void operator&() const;
} nullptr_t = {};

int main(int argc, char *argv[])
{
	int a = 8;
	printf("nullptr_t = %d\n", (nullptr_t_t::*)a);
	printf("a = %d\n", a);

	return 0;
}
