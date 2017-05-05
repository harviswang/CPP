#include <iostream>
#include <exception>
#include <cstdlib> // abort()
#include <cstdio> // snprintf()
#include <cfloat>

using namespace std;
namespace ceph {
  /*
   * exceptions
   */

  struct error : public std::exception{
    const char *what() const throw () override;
  };
  struct bad_alloc : public error {
    const char *what() const throw () override;
  };
  struct end_of_buffer : public error {
    const char *what() const throw () override;
  };
  struct malformed_input : public error {
    explicit malformed_input(const std::string& w) {
      snprintf(buf, sizeof(buf), "buffer::malformed_input: %s", w.c_str());
    }
    const char *what() const throw () override;
  private:
    char buf[256];
  };
  struct error_code : public malformed_input {
    explicit error_code(int error);
    int code;
  };

  const char * error::what() const throw () {
    return "buffer::exception";
  }
  const char * bad_alloc::what() const throw () {
    return "buffer::bad_alloc";
  }
  const char * end_of_buffer::what() const throw () {
    return "buffer::end_of_buffer";
  }
  const char * malformed_input::what() const throw () {
    return buf;
  }
}


int main()
{
	int x = 0;
	if (x == 0) {
		try {
			throw ceph::end_of_buffer();
		} catch (const char *str) {
			cout << str << endl;
		} catch (...) {
			cout << " catch(...) " << endl;
		}
	}

	return 0;
}
