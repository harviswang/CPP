#include <iostream>

using namespace std;

int main()
{
	pair<string,string> p = make_pair("y", "Y");
	cout << "pair.first " << p.first << " pair.second " << p.second << endl;

	pair<string, string> empty_pair = pair<string, string>();
	cout << "empty_pair.first " << empty_pair.first 
		<< " empty_pair.second " << empty_pair.second << endl;

	return 0;
}
