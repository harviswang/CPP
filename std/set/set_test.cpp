#include <iostream>
#include <set>

using namespace std;

int main()
{
	cout << "set test ..." << endl;
	set<int> num;
	num.insert(0);
	num.insert(1);
	cout << num.size();
	return 0;
}
