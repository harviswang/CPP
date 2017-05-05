#include <iostream>
#include <vector>
#include <map>
#include <vector>

using namespace std;

int main()
{
	vector<int> week(7);
	vector<int> vec(7);

	for (int i = 0; i < 7; i++) {
		week[i] = i;
	}
	copy(week.begin(), week.end(), vec.begin());
	for (vector<int>::const_iterator itor = vec.begin(); itor != vec.end(); itor++)
	{
		cout << *itor << endl;
	}

	map<int, int> m;
	vector<pair<int, int> > pa(7);
	m.insert(make_pair(0, 0));
	m.insert(make_pair(1, 1));

	//copy(m.begin(), m.end(), inserter(pa, pa.begin()));
	copy(m.begin(), m.end(), pa.begin());
	for (vector<pair<int, int> >::const_iterator itor = pa.begin(); itor != pa.end(); itor++)
	{
		cout << itor->first << " " << itor->second  << endl;
	}

	return 0;
}
