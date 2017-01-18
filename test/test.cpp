#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

struct A{
   A(int ia, int ib){ a = ia; b = ib;}

	int a;
	int b;
};

int main(int argc, char *argv[])
{
	int *buf = (int*)calloc(128, sizeof(int));
	printf("test ...\n");
	A *pa = new(buf) A(3, 5);
	printf("buf[0]=%d\n", buf[0]);
	printf("buf[1]=%d\n", buf[1]);
	printf("pa->a=%d\n", pa->a);
	printf("pa->b=%d\n", pa->b);
        std::cout << "pa=" << pa << std::endl;
        std::cout << "buf=" << buf << std::endl;
	free(buf);
	return 0;
}
