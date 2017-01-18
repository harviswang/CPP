#include <stdio.h>

class IInteraction{
public:
    IInteraction() { }
    ~IInteraction() { }
    static int *s_instance;
};

int main()
{
	printf("IInteraction::s_instance = %p\n", IInteraction::s_instance);
	return 0;
}
