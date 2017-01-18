#include <iostream>
using namespace std;

static void constructor_protected_test(void);
static void constructor_assignment_test(void);
static void constructor_assignment_new_test(void);
static void constructor_private_test(void);
extern void copy_constructor_test(void);
static void constructor_subclass_test(void);

int main(int argc, char **argv)
{
	constructor_assignment_test();
	constructor_assignment_new_test();
	constructor_private_test();
	copy_constructor_test();
	constructor_protected_test();
	constructor_subclass_test();

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

/*
 * 设计模式singleton模式的一个简单的C++代码实现
 * static成员方法的调用可以不需要通过一个对象, 直接通过class_name::static_function()即可.
 */
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton s;
        return s;
    }
private:
    Singleton(){}
public:
    ~Singleton(){}
    void fun() {
        cout << __func__ << endl;
    }
};

/*
 * 使用指针和引用, 生成的a.out的大小一样
 * 对于singleto模式必须使用引用
 */
static void constructor_private_test(void)
{
    Singleton *s = &Singleton::getInstance();
    s->fun();
    cout << s << endl;

    Singleton *s2 = &Singleton::getInstance();
    s2->fun();
    cout << s2 << endl;
}

/*
 * protected修饰构造函数对类外调用是屏蔽的, 对派生类是透明的
 */
class Computer {
protected:
    Computer() {
        cout << __func__ << "(), Line:" << __LINE__ << endl;
    }
    ~Computer() {
        cout << __func__ << "(), Line:" << __LINE__ << endl;
    }
};
class Thinkpad: public Computer
{
public:
    Thinkpad() {cout << "Thinkpad constructor\n";}
};

static void constructor_protected_test(void)
{
    cout << __func__ << " test ...\n";
    Thinkpad e430;
}

class Society {
public:
    Society() { }
    Society(Society* other) :
        unemployment_rate(other->unemployment_rate) {

    }
    Society(float rate) { unemployment_rate = rate; }
    ~Society() { }
    float get_unemployment_rate() {
        return unemployment_rate;
    }

    void set_unemployment_rate(float rate) {
            unemployment_rate = rate;
    }
private:
    float unemployment_rate;
};

class ChinaSociety : public Society {
public:
    ChinaSociety(){ }
    ChinaSociety(Society *parent) :
        Society(parent),
        society(new ChinaSociety()) {
    }
    ~ChinaSociety() {
        delete society;
    }

    void dump() {
        cout << "unemployment_rate = " << society->get_unemployment_rate() << endl;
    }
private:
    Society *society;
};

/*
 * 子类构造函数中调用父类的构造函数
 */
static void constructor_subclass_test(void)
{
    Society *s = new Society(0.05);
    ChinaSociety *cs = new ChinaSociety(s);
    cs->dump();
    delete cs;
    delete s;
}
