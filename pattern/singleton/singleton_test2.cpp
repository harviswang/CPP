#include <stdio.h>

class SingletonInterface
{
public:
	virtual void start() = 0;
	virtual ~SingletonInterface() {}
protected:
	static SingletonInterface *m_instance;
};

class A : public SingletonInterface
{
public:
	void start() { printf("A::start()\n");	}
	static SingletonInterface* getInstance() {
		if (m_instance == NULL) {
			m_instance = new A();
		}
		return m_instance;
	}
};

class B : public SingletonInterface
{
public:
	void start() { printf("B::start()\n");	}
	static SingletonInterface* getInstance() {
		if (m_instance == NULL) {
			m_instance = new B();
		}
		return m_instance;
	}
};

int main(int argc, char *argv[])
{
	printf("singleton pattern test...\n");
	A a;
	B b;
	a.start();
	b.start();
	a.getInstance();
	//A *a = A::getInstance();
	//printf("A::getInstance() = %p\n", A::getInstance());
	//B *b = B::getInstance();
	//printf("B::getInstance() = %p\n", B::getInstance());
	
	return 0;
}

