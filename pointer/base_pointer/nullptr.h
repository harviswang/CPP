// 某些编译器不支持c++11的新关键字nullptr，我们也可以模拟实现一个nullptr。
// http://blog.csdn.net/huang_xw/article/details/8764346

#ifndef __NULLPTR_H__
#define __NULLPTR_H__

const class nullptr_user_t 
{
public:
	template<class T>          operator T*() const { return 0; }
	template<class C, class T> operator T C::*() const { return 0; }
private:
	void operator &() const;
} nullptr_user = {};

#ifdef NULL
#undef NULL
#define NULL nullptr_user
#endif
#endif // __NULLPTR_H__
