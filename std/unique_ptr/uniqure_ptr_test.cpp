#include <iostream>

using namespace std;

struct name_t {
	string first_name;
	string middle_name;
	string last_name;

	name_t() : first_name(""),
                  middle_name(""),
		  last_name("") {}
	void set_first_name(string _f) {
		first_name = _f;
	}
};

int main()
{
	unique_ptr<name_t> x(new name_t);
	cout << x << endl;
	x->set_first_name("Mr");
	cout << x->first_name << endl;

	return 0;
}
