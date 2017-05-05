#include <iostream>
#include <list>

using namespace std;

static void list_iterator_test();

int main()
{
	list<int> a;
	a.push_back(1);
	a.push_back(2);

	a.pop_front();
	cout << a.front();

	list_iterator_test();
	return 0;
}

static void list_iterator_test()
{
	list<int> l;
	for (int i = 0; i < 10; i++) {
		l.push_back(i*i);
	}

	for (list<int>::iterator iter = l.begin(); iter != l.end(); iter++) {
		cout << *iter << endl;
	}
}
