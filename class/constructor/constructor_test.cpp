#include <iostream>
using namespace std;

static void constructor_assignment_test(void);
static void constructor_assignment_new_test(void);
int main(int argc, char **argv)
{
	constructor_assignment_test();
	constructor_assignment_new_test();
	return 0;
}

static void constructor_assignment_test(void)
{
class List {
public:
	int a;
	int b;
	int c;
};
	List list = {1, 2, 3};
	cout << "list.a = " << list.a << "\n";
	cout << "list.b = " << list.b << "\n";
	cout << "list.c = " << list.c << "\n";
	cout << "\n";
}

static void constructor_assignment_new_test(void)
{
class Time {
private:
	int m_year;
	int m_month;
	int m_day;
public:
	Time() {
		cout << __func__ << ": " << __LINE__ << "\n";
	}; // non-private
	Time(int year, int month = 0, int day = 0) {
		m_year = year;
		m_month = month;
		m_day = day;
	} 
	void dump() {
		cout << "year: " << m_year << "\n";
		cout << "month: " << m_month << "\n";
		cout << "day: " << m_day << "\n";
		cout << "\n";
	}
};

//	Time time = Time; // Error
	Time time;
	time.dump();//out are random number	

	Time t(2);
	t.dump();

	Time today = Time(2016, 5, 8);
	today.dump();

	Time *birthday = new Time(2001, 10);
	birthday->dump();
	delete birthday;
}
