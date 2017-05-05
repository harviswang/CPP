// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

// 'auto' not allowed in function prototype
// c++11 feature
// Syntax:
// auto declarator initalizer;
// [](auto param1, auto param2){};
// 'auto' not allowed in lambda parameter
// 'auto' not allowed in function prototype
//

#include <iostream>
#include <map>
#include <list>
#include <typeinfo>
#include <deque>

using namespace std;

static void auto_simple_usage_test();
static void auto_iterator_test();
static void auto_param_test();
static void auto_reference_test();
static void auto_for_test();
static void auto_new_test();
static void auto_conditional_operator_test();
static void auto_function_pointer_test();

#define AUTO_TYPE_NANE(x) (typeid(x).name())

int main()
{
  auto_simple_usage_test();
  auto_iterator_test();
  auto_param_test();
  auto_reference_test();
  auto_for_test();
  auto_new_test();
  auto_conditional_operator_test();
  auto_function_pointer_test();

  return 0;
}

static void auto_simple_usage_test()
{
  auto num = 5;
  cout << sizeof(num) << endl;
  const type_info &ti = typeid(num);
  cout << ti.name() << endl;

  auto numl = 5l;
  cout << sizeof(numl) << endl;

  auto numll = 5ll;
  cout << sizeof(numll) << endl;
}

static void auto_iterator_test()
{
  map<string, list<int>> records = {
    {"ZhangSan", {80, 60, 98}},
    {"LiSi", {36, 21, 63}}
  };
  auto iter = records.find("LiSi");
  // map<string, list<int>>:iterator iter = records.find("LiSi");
  cout << AUTO_TYPE_NANE(iter) << endl;
  cout << AUTO_TYPE_NANE(records) << endl;
}

static void auto_param_test()
{
}

static void auto_reference_test()
{
  int count = 10;
  int& countRef = count;
  auto myAuto = countRef; // myAuto is an int
  //auto& myAuto = countRef; // myAuto is an int reference

  countRef = 11;
  cout << count << " ";

  myAuto = 12;
  cout << count << " ";

  cout << sizeof(myAuto) << " " << sizeof(countRef) << endl;
}

static void auto_for_test()
{
  deque<double> dqDoubleData(10, 10.2);
  for (auto elem : dqDoubleData) {
    cout << elem << " ";
  }
  cout << endl;

  for (auto& elem : dqDoubleData) {
    cout << elem << " ";
  }
  cout << endl;
}

static void auto_new_test()
{
  double x = 12.34;
  auto *y = new auto(x), **z = new auto(&x);
  cout << *y << " " << **z << endl;
}

static void auto_conditional_operator_test()
{
  int v1 = 100, v2 = 200;
  auto x = v1 > v2 ? v1 : v2;
  cout << x << endl;
}

static int f(int x) { return x; }
//static decltype(auto) f(int x) { return x; } // c++14
static void auto_function_pointer_test()
{
  auto x = f(0); // x int
  const auto & y = f(1); // y const int
  int (*p)(int x);
  p = f;
  auto fp = p;
  cout << x << " " << y << " " << fp(9) << endl;
}
