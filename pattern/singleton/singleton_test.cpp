/*
 * Singleton单例模式
 * Singleton 是对全局变量的取代策略
 * 作用：保证一个类只能有一个实例，并提供一个全局唯一的访问点。
 */
#include <stdio.h>

class Singleton
{
public:
	void dump() { printf("Singleton::dump()\n"); }
	static Singleton& getInstance() {
		static Singleton m_instance;
		return m_instance;
	}
private:
	Singleton() {}
	Singleton(const Singleton&); 
	const Singleton& operator=(const Singleton&); 
};

int main(int argc, char *argv[])
{
	printf("singleton pattern test...\n");
	Singleton::getInstance().dump();
	return 0;
}

