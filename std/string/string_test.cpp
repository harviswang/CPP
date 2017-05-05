#include <string>
#include <string.h>
#include <iostream>
#include <sstream>

using namespace std;

static bool is_valid_crush_name(const string& s);
static bool is_valid_crush_name(const char *str);
static std::string cpp_strerror(int err);
static string consolidate_whitespace(string in);
static void string_erase_test(void);
static void string_stringify_test(void);
static void string_to_string_test(void);

int main()
{
	const char *args = "color=red";
	char *p = strchr(args, '=');
	string str(args);
	cout << "length(" << str.c_str() << ") " <<  str.length() << endl;

	string key(args, 0, p - args);
	cout << "key " << key << endl;

	string value(p + 1);
	cout << "value " << value << endl;

	// find test
	size_t pos = str.find('=');
	if (pos != string::npos) {
		cout << "pos " << pos << endl;
		cout << "string::npos " << string::npos << endl;
	}

	// substr test [0, pos0
	cout << str.substr(0, pos) << endl;
	cout << str.substr(pos + 1) << endl;

	if (is_valid_crush_name(string("crush"))) {
		cout << "ok" << endl;
	}

	const char *osd = "osd.4";
	if (is_valid_crush_name(osd)) {
		cout << "ok" << endl;
	}

	for (auto &ch : string(osd)) {
		cout << ch << " ";
	}
	cout << endl;

	cout << cpp_strerror(-1) << endl;

	consolidate_whitespace("   work   hard to get more   ");

	string_erase_test();

	string_stringify_test();
	string_to_string_test();
	return 0;
}

// check s is only consisted of set [A-Za-z0-9_-.]
static bool is_valid_crush_name(const string& s)
{
  if (s.empty())
    return false;
  for (string::const_iterator p = s.begin(); p != s.end(); ++p) {
    if (!(*p == '-') &&
        !(*p == '_') &&
        !(*p == '.') &&
        !(*p >= '0' && *p <= '9') &&
        !(*p >= 'A' && *p <= 'Z') &&
        !(*p >= 'a' && *p <= 'z'))
      return false;
  }
  return true;
}

static bool is_valid_crush_name(const char *str)
{
	cout << "bool is_valid_crush_name(const char *str)" << endl;
	return is_valid_crush_name(string(str));
}

static std::string cpp_strerror(int err)
{
  char buf[128];
  char *errmsg;

  if (err < 0) {
    err = -err;
  }
  // TODO
  std::ostringstream oss;
  buf[0] = '\0';

  // strerror_r returns char * on Linux, and does not always fill buf
#ifdef STRERROR_R_CHAR_P
  errmsg = strerror_r(err, buf, sizeof(buf));
#else
  strerror_r(err, buf, sizeof(buf));
  errmsg = buf;
#endif

  oss << "(" << err << ") " << errmsg;

  return oss.str();
}

// squash runs of whitespace to one space, excepting newlines
static string consolidate_whitespace(string in)
{
  string out;

  bool white = false;
  for (unsigned p=0; p<in.length(); p++) {
    if (isspace(in[p]) && in[p] != '\n') {
      if (white)
        continue;
      white = true;
    } else {
      if (white) {
        if (out.length()) out += " ";
        white = false;
      }
      out += in[p];
    }
  }

  cerr << " \"" << in << "\" -> \"" << out << "\"" << std::endl;

  return out;
}

static void string_erase_test()
{
	string str("newline\n");
	cout << str;
	str.erase(str.length() - 1, 1);
	str.append("\n");
	cout << str;
}

template<typename T>
inline std::string stringify(const T& a) {
#if defined(__GNUC__) && !(defined(__clang__) || defined(__INTEL_COMPILER))
  static __thread std::ostringstream ss;
  ss.str("");
#else
  std::ostringstream ss;
#endif
  ss << a;
  return ss.str();
}

static void string_stringify_test()
{
	int a = 1024;
	cout << stringify<int>(a);
	cout << endl;
}

static void string_to_string_test(void)
{
	int x = 2;
	cout << std::to_string(x) << endl;
}
