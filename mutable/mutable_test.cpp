// mutalbe的中文意思是“可变的，易变的”，跟constant（既C++中的const）是反义词。
// 在C++中，mutable也是为了突破const的限制而设置的。被mutable修饰的变量，将永远处于可变的状态，
// 即使在一个const函数中。
// 如果类的成员函数不会改变对象的状态，那么这个成员函数一般会声明成const的。但是，有些时候，
// 需要在const的函数里面修改一些跟类状态无关的数据成员，那么这个数据成员就应该被mutalbe来修饰
#include <iostream>

class MutableTest {
	public:
		MutableTest();
		~MutableTest();

		void Output() const;
		int GetOutputTimes() const;

	private:
		mutable int m_outputTimes;
};

MutableTest::MutableTest()
{
	m_outputTimes = 0;
}

MutableTest::~MutableTest()
{}

void MutableTest::Output() const
{
	std::cout << "Output ..." << std::endl;
	m_outputTimes++; // mutable突破const的限制
}

int MutableTest::GetOutputTimes() const
{
	return m_outputTimes;
}

int main()
{
	MutableTest m;
	m.Output();
	std::cout << m.GetOutputTimes() << std::endl;

	return 0;
}
