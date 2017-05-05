#include <iostream>
#include <vector>

#include <boost/icl/interval_map.hpp>

using namespace std;
using namespace boost::icl;

int main()
{
	interval_map<int, int> rule;
	rule.insert(make_pair(0, 1));

	return 0;
}
