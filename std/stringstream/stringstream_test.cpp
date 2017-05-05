// ios_base <- ios <- istream/ostream <- iostream <- stringstream

#include <iostream>
#include <vector> // vector
#include <sstream> // stringstream

using namespace std;

static void stringstream_in_out_test();

int main()
{

	stringstream ss;
	ss.clear();
	ss.str("a test stringstream variable");
	cout << ss.str() << endl;
	stringstream_in_out_test();

	return 0;
}

static void
write_integer_indexed_vector_data_string(vector<string> &dst, int index, vector<int> vector_data)
{
  //stringstream data_buffer (stringstream::in | stringstream::out);
  stringstream data_buffer (stringstream::out);

  unsigned input_size = vector_data.size();

  // pass the indexing variable to the data buffer
  data_buffer << index;

  // pass the rest of the input data to the buffer
  for (unsigned i = 0; i < input_size; i++) {
    data_buffer << ',' << vector_data[i];
  }

  data_buffer << std::endl;

  // write the data buffer to the destination
  dst.push_back( data_buffer.str() );
}

static void stringstream_in_out_test()
{
	vector<string> dst;
	vector<int> data;

	data.push_back(1);
	data.push_back(2);

	write_integer_indexed_vector_data_string(dst, 0, data);
	cout << dst.back();
}
