// Multi-key map
//
#include <iostream>
#include <map>

int main()
{
	std::multimap<int, std::string> table;

	table.insert(std::pair<int, std::string>(0, "hello"));
	table.insert(std::make_pair(1, "word"));
	std::multimap<int, std::string>::iterator iter = table.find(-1);
	if (iter == table.end()) {
		std::cout << "find failed!" << std::endl;
	}
	// not support << operator
	//std::cout << "multimap content are (" << table << std::endl;
	//
	// swap test
	std::multimap<int, std::string> ismap;
	ismap.swap(table);
	std::cout << ismap.begin()->first << std::endl;
	std::cout << table.begin()->first << std::endl;
	return 0;
}
