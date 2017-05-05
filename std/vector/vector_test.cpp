// -*- mode:C++; tab-width:8; c-basic-offset:2; indent-tabs-mode:t -*-
// vim: ts=8 sw=2 smarttab

// vector iterator: Random Access Iterator
#include <iostream>
#include <vector>
#include <random>

using namespace std;

static void vector_iterator_test(void);
static void vector_swap_test(void);
static void vector_back_test(void);
static void vector_assign_test(void);

int main()
{
	vector_iterator_test();
	vector_swap_test();
	vector_back_test();
	vector_assign_test();
	
	return 0;
} 

static void vector_iterator_test()
{
  vector<int> ivec(10, 1);
  for (vector<int>::iterator iter = ivec.begin(); iter != ivec.end(); iter++) {
    *iter = iter - ivec.begin();
  }
  cout << ivec[8] << endl;
}

static void vector_swap_test()
{
  int array[] = {1,2,3,4,5,6,7};
  vector<int> a(array, array + sizeof(array) / sizeof(array[0]));

    // permute bucket list
    for (unsigned i = 0; i < a.size(); i++) {
      unsigned j = lrand48() % (a.size() - 1);
      swap(a[i], a[j]);
    }

    for (unsigned i = 0; i < a.size(); i++) {
	    cout << a[i] << " ";
    }
    cout << endl;
}

static void vector_back_test()
{
  vector<string> affected_types;
  affected_types.push_back("bucket");

  cout << affected_types.back() << endl;
  cout << affected_types.front() << endl;
}

static void vector_assign_test()
{
  vector<int> vec(10);
  vector<int> clone;
  int arr[10];

  for (int i = 0; i < 10; i++) {
    arr[i] = 2 * i;
  }

  vec.assign(arr, arr + 10);

  clone.assign(vec.begin(), vec.end());
  cout << vec[9] << endl;
  cout << clone[9] << endl;
}
