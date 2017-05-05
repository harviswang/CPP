// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

#include <vector>
#include <boost/spirit.hpp>

#ifdef USE_BOOST_SPIRIT_OLD_HDR
#include <boost/spirit/core.hpp>
#include <boost/spirit/tree/ast.hpp>
#include <boost/spirit/tree/tree_to_xml.hpp>
#else
#define BOOST_SPIRIT_USE_OLD_NAMESPACE
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_ast.hpp>
#include <boost/spirit/include/classic_parse_tree.hpp>
#endif

using namespace boost::spirit;
using namespace std;

bool
parse_numbers(char const* str)
{
  return parse(str, real_p >> *(ch_p(',') >> real_p), space_p).full;
}

bool
parse_numbers(char const* str, vector<double>& v)
{
  return parse(str,
      // Begin grammar
      (
               real_p[append(v)] >> *(ch_p(',') >> real_p[append(v)])
      )
            // End grammar
      ,space_p).full;
}

int main()
{
  parse_numbers("2.0,3.14");
  vector<double> dv;
  if (parse_numbers("1,2,3", dv)) {
    for (vector<double>::const_iterator it = dv.begin(); it != dv.end(); it++) {
      cout << *it << endl;
    }
  }

  return 0;
}

