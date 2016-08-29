/*
 * 在C++中，静态成员是属于整个类的而不是某个对象，静态成员变量只存储一份供所有对象共用。
 * 静态成员的定义或声明要加个关键static。静态成员可以通过双冒号来使用即<类名>::<静态成员名>。
 * 1. 静态成员函数中不能调用非静态成员
 * 2. 非静态成员函数中可以调用静态成员
 *    因为静态成员属于类本身，在类的对象产生之前就已经存在了，
 *    所以在非静态成员函数中是可以调用静态成员的。
 * 3. 静态成员变量使用前必须先初始化(如int MyClass::m_nNumber = 0;)，否则会在linker时出错
 */
#include <stdio.h>
#include <string.h>

static void point_test();
static void student_test();
int main(int argc, char *argv[])
{
	printf("static method, static function test...\n");

	point_test();
	student_test();

	return 0;
}

class Point {
public:
	Point() { m_nPointCount++; }
	~Point() { m_nPointCount--; }
	void init() {}
	void init2() { output(); }
	static void output() {}
	static void dumpPointCount() { 
		printf("m_nPointCount = %d\n", m_nPointCount);
	}
//	static void dump() { printf("m_x = %d\n", m_x); }
private:
	int m_x;
	static int m_nPointCount;
};
// 类的静态成员变量在使用前必须先初始化
int Point::m_nPointCount = 0;

static void point_test()
{
	// 不能通过类名来调用类的非静态成员函数
	//Point::init();
	
	// 通过类名来调用类的静态成员函数
	Point::output();
	
	// 类的对象可以使用静态成员函数和非静态成员函数
	Point pt;
	pt.init();
	pt.output();
	
	// 在类的静态成员函数中使用类的非静态成员
	// 静态成员函数中不能引用非静态成员
	Point pt2;
	//pt2.dump();
	(void)pt2;
	
	// 在类的非静态成员函数中使用类的静态成员
	// 类的非静态成员函数可以调用用静态成员函数，但反之不能
	Point pt3;
	pt3.init2();
	
	// 使用类的静态成员变量
	Point pt4;
	pt4.dumpPointCount();
}

class Student {
	enum { MAX_NAME_SIZE = 10 };
public:
	Student(const char *pszName);
	~Student();
public:
	static void printAllStrudents();
private:
	char m_name[MAX_NAME_SIZE];
	Student *next;
	Student *prev;
	static Student *m_head;
};

Student::Student(const char* pszName)
{
	strcpy(this->m_name, pszName);
	
	/*
	 * m_head->prev <---> this <---> m_head
	 */
	if (this->m_head != NULL) {
		this->next = this->m_head;
		this->prev = this->m_head->prev;
		this->m_head->prev->next = this;
		this->m_head->prev = this;
	} else {
		this->next = this;
		this->prev = this;
		this->m_head = this;
	}
}

Student::~Student()
{
	if (this == this->m_head) {
		this->m_head = this->next;
	}
	this->prev->next = this->next;
	this->next->prev = this->prev;
}

void Student::printAllStrudents()
{
	Student *p = m_head;
	
	do {
		printf("name : %s\n", p->m_name);
		p = p->next;
	} while (p != m_head);
}

Student* Student::m_head = NULL;

static void student_test()
{
	Student xiaoming("XiaoMing");
	Student lily("Lily");
	Student hanmeimei("HanMeimei");
	lily.printAllStrudents();
}