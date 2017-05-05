// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

// [captures](params)->ret {statments;}
// captures
//    [] 不截取任何变量
//    [&} 截取外部作用域中所有变量，并作为引用在函数体中使用
//    [=] 截取外部作用域中所有变量，并拷贝一份在函数体中使用
//    [=, &foo]   截取外部作用域中所有变量，并拷贝一份在函数体中使用，但是对foo变量使用引用
//    [bar]   截取bar变量并且拷贝一份在函数体重使用，同时不截取其他变量
//    [this]            截取当前类中的this指针。如果已经使用了&或者=就默认添加此选项。


#include <iostream>
#include <list>
#include <functional>

using namespace std;
static void lambda_funtion_test();
static void lambda_as_argument_test();
static void lambda_nested_test();

int main()
{
  auto func = []() { cout << __PRETTY_FUNCTION__ << endl; };
  func();

  lambda_funtion_test();
  lambda_as_argument_test();
  lambda_nested_test();

  return 0;
}

  template <typename L, typename func_t>
int list_find(list<L> l, func_t func)
{
  int pos = -1;
  int index = 0;

  for (auto iter = l.begin(); iter != l.end(); iter++, index++) {
    if (func(*iter)) {
      pos = index;
    }
  }

  return pos;
}

static void lambda_funtion_test()
{
  list<int> l = {1, 2, 3, 4, 5};
  int find = list_find(l, [](int x){ return x == 5; });
  cout << find << endl;
  // using for_each travers a list
  for_each(l.begin(), l.end(), [](int val) { cout << val << " "; });
  cout << endl;
  
  auto f1 = [](int x, int y) { return x+y; };
  cout << f1(2, 3) << endl;

  function<int(int, int)> f2 = [](int x, int y) { return x+y; };
  cout << f2(3, 4) << endl; 

  int i = 3;
  int j = 5;
  function<int (void)> f = [i, &j] { return i+j; };
  i = 22;
  j = 44;
  cout << f() << endl;

  int n = [](int x, int y) { return x+y; }(5, 4);
  cout << n << endl;
}

static void lambda_as_argument_test()
{
  list<int> numbers;
  numbers.push_back(13);
  numbers.push_back(17);
  numbers.push_back(42);
  numbers.push_back(46);
  numbers.push_back(99);

  const auto result = find_if(numbers.begin(), numbers.end(),
      [](int n) { return (n % 2) == 0; });
  if (result != numbers.end()) {
    cout << "The first even number in the list is " << *result << endl;
  } else {
    cout << "The list contains no even numbers." << endl;
  }
}

static void lambda_nested_test()
{
  int x = [](int x, int y) { return [](int i, int j) { return i * j; } (x, y);} ( 8, 9);
  cout << x << endl;
}
