// -*- mode:C++; tab-width:4; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=4 sw=2 smarttab

#include <iostream>
#include <map>

using namespace std;

static void build_rmap(map<int, string> &f, map<string, int> &r);
static void map_iterator_test(void);
static void map_find_test(void);
static void map_insert_test(void);
static void map_value_type_test(void);

int main()
{
	map_iterator_test();
	map_find_test();
	map_insert_test();
	map_value_type_test();
	return 0;
}

static void map_iterator_test()
{
	map<int, string> week;
	map<string, int> rweek;
	week[0]="Sunday";
	week[0]="Sunday2"; // override the last value
	week[1]="Monday";
	week[2]="Tuesday";
	week[3]="Wednesday";
	week[4]="Thursday";
	week[5]="Friday";
	week[6]="Saturday";

	for (map<int, string>::const_iterator itor = week.begin();
			itor != week.end(); itor++) {
		cout << "key[" << itor->first << "]=";
		//cout << itor->second << endl;
		cout << (*itor).second << endl;
	}

	build_rmap(week, rweek);
	for (map<string, int>::const_iterator itor = rweek.begin(); itor != rweek.end(); itor++) {
		cout << "key[" << itor->first << "]=";
		cout << itor->second << endl;
	}

}

// reverse a map, map<int, string> -> map<string, int>
static void build_rmap(map<int,string> &f, map<string, int> &r)
{
	r.clear();
	for (map<int,string>::const_iterator itor = f.begin(); itor != f.end(); itor++) {
		r[itor->second] = itor->first;
	}
}

static void map_find_test()
{
	map<int, int> price;
	price.insert(pair<int, int>(1, 8));
	price[2]=16;
	map<int, int>::const_iterator loc = price.find(1);
	cout << "first " << loc->first << " second " << loc->second << endl;
}

static void map_insert_test()
{
	map<int, string> x;	
	pair<map<int, string>::iterator, bool> ret;
	ret = x.insert(make_pair(0, "zero"));
	if (ret.second == true) {
		cout << "insert <" << ret.first->first << "," 
			<< ret.first->second << ">" << endl;
		ret = x.insert(make_pair(0, "zero"));
		if (ret.second == false) {
			cout << "element alread existed ";
	       		cout << "with a value of " << ret.first->second << endl;
		}
	}
}

static void map_value_type_test()
{
	map<string, string> name;
	map<string, string>::value_type mr("Li", "Si");
	name.insert(mr);
}
