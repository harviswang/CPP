#include <iostream>

using namespace std;

void Throw() {
	throw __PRETTY_FUNCTION__;
}

void NoBlockThrow() {
	Throw();
}

void BlockThrow() noexcept {
	Throw();
}

int main()
{
	try {
		Throw();
		NoBlockThrow();
	} catch (const char *str) {
		cout << str << endl;
	}

	try {
		BlockThrow();
	} catch (const char *str) {
		cout << str << endl;
	}


	return 0;	
}
