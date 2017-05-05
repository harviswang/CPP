// 1. friend static xxxx is invalid
// 2. friend can be public, private and both if you like
// 3. friend can't be inherited
// 4. friend doesn't exchangeability
#include <iostream>

using namespace std;

static void friend_function_test();
static void friend_class_test();

int main()
{
	friend_function_test();
	friend_class_test();

	return 0;
}

class Point {
	private:
		double x;
		double y;
		Point(double x_, double y_) : x(x_), y(y_) {}
	public:
		Point():x(0.0), y(0.0){}
		~Point(){}

	public:
		friend void Dump(Point &p); 
	private:
		friend void Dump(Point &p); 
		friend class Rectangle;
};

void Dump(Point &p) {
	std::cout << "x " << p.x << " y " << p.y << endl;
}

static void friend_function_test()
{
	Point p;
	Dump(p);	
}

class Rectangle {
	Point mLowerLeft;
	Point mTopRight;
public:

	Rectangle(Point &ll, Point &ur) : mLowerLeft(ll), mTopRight(ur) {}
	Rectangle(double x1, double y1, double x2, double y2) {
		mLowerLeft = Point(x1, y1);
		mTopRight = Point(x2, y2);
	}
	~Rectangle() {}

	double getArea() {
		double ret = (mLowerLeft.x - mTopRight.x) *
			(mLowerLeft.y - mTopRight.y);
		return ret > 0 ? ret : -ret;
	}
};

static void friend_class_test()
{
	Rectangle rec(0, 0, 2, 2);
	std::cout << rec.getArea() << std::endl;
}
