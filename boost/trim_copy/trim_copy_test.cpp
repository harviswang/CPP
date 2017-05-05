#include <boost/algorithm/string.hpp>
#include <iostream> // cout

// boost::trim_copy() remove prefix/suffix empty characters

int main()
{
	std::string str("   boost string    ");
	if (boost::trim_copy(str) == boost::trim_copy(std::string("boost string"))) {
		std::cout << boost::trim_copy(str) << std::endl;
	}
	return 0;
}
