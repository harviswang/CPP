#include <iostream>
#include <map>

static void build_rmap(std::map<int, std::string> &f, std::map<std::string, int> &r);

int main()
{
	std::map<int, std::string> week;
	std::map<std::string, int> rweek;
	week[0]="Sunday";
	week[1]="Monday";
	week[2]="Tuesday";
	week[3]="Wednesday";
	week[4]="Thursday";
	week[5]="Friday";
	week[6]="Saturday";
	week[7]="Sunday2";

	std::cout << "std::map test\n";
	for (std::map<int, std::string>::const_iterator itor = week.begin(); itor != week.end(); itor++) {
		std::cout << "key[" << itor->first << "]=";
		std::cout << itor->second << std::endl;
	}

	build_rmap(week, rweek);
	rweek.erase(week[7]);
	for (std::map<std::string, int>::const_iterator itor = rweek.begin(); itor != rweek.end(); itor++) {
		std::cout << "key[" << itor->first << "]=";
		std::cout << itor->second << std::endl;
	}

	std::cout << rweek.count(week[5]) << std::endl;
	return 0;
}

// reverse a map, map<int, string> -> map<string, int>
static void build_rmap(std::map<int,std::string> &f, std::map<std::string, int> &r)
{
	r.clear();
	for (std::map<int,std::string>::const_iterator itor = f.begin(); itor != f.end(); itor++) {
		r[itor->second] = itor->first;
	}
}
