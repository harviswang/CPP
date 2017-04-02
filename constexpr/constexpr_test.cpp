// The keyword constexpr was introduced in C++11 and improved in C++14.
// It means constant expression. Like const, it can be applied to variables
// so that a compiler error will be raised if any code attempts to modified
// the value. Unlike const, constexpr can also be applied to functions and
// class constructors. constexpr indicates that the value, or return value,
// is constant and, if possible, will be computed at compile time. A constexpr
// integral value can be used wherever a const integer is required, such as
// in template arguments and array declarations. And when a value can be comp-
// uted at compile time instead of run time, it can help your progrm can run
// faster and use less memory.
// Syntax:
// constexpr  literal-type  identifier = constant-expression;constexpr  literal-type  identifier { constant-expression };constexpr literal-type identifier(params );constexpr ctor (params);  
// Parmeters:
// params: one or more parameters which must be a literal type(as listed below)
//         and must itself be a constant expression.
// Return Value:
// A constexpr varible or function must return one of the literal types, as listed below
// Literal types:
// To limit the complexity of computing compile time constants, and their potential
// impacts of compilation time, the C++14 standrd requires that the types involved
// in constant expressions be restricted to literal types. A literal type is one of
// whose layout can be determined at compile time, The following are the literal types:
// 1. void
// 2. scalar types
// 3. references
// 4. Arrays of void, scalar types or references
// 5. A class that has a trivial destructor, and one or more constexpr constructor
//    that are not move or copy constructors. Additionally, all its non-static data
//    members and base classes must be literal types and not volatile.
//
// General constexpr rules
// For a function variable, constructor or static member to be defined as constexpr,
// it must meet certain requirements:
// * A constexpr function can be recursive, It cannot be virtual, and its return type
//   and parameter types must all be literal types. The body can be defined as = default
//   or = delete. Otherwise it must follow these rules: it contains no goto statements,
//   try blocks, unitialized variables, or variable definitions that are not literal types,
//   or that are static or thread-local. Additionally, a consturctor cannot be defined
//   as constexpr if the enclosing class has any virtual base class.
// * A variable can be declared with constexpr, if it has a literal type and is initialized.
//   If the initialization is performed by a constructor, the constructor must be declared
//   as constexpr.
// * A reference may be declared as constexpr if the object that it references has been initilized
//   by constant expression and any implicit conversions that are invoked during initialization 
//   are also constant expressions.
// * All declarations of a constexpr variable or function must have the constexpr specifier.
// * An explicit specialization of a non-constexpr template can be declared as constexpr.
// * An explicit specialization of a constexpr template does not have to also be constexpr.
// * A constexpr function or constructor is implicitly inline.
//
// [Question] error: unknown type name 'constexpr'
// [Answer] compile need option '-std=c++11'
//
#include <iostream>

static void numeric_limits_test(void);

// constexpr functions
// A constexpr function is one whose return value can be computed at compile
// when consuming code requries it. A constexpr function must accept and return
// only literal types. When its arguments are constexpr values, and consuming
// requires the return value at compile time, for example to initialize a constexpr
// arguments, or when its value is not required at compile-time, it produces a 
// value at run time like a regular function.(This dual behavior saves your from
// having to write constexpr and non-constexpr versions of the same function.)
// Can't make an constexpr function declaration then make an constexpr funciton
// implementation
// Pass by value
static constexpr float exp(float x, int n)
{
	return n == 0 ? 1 :
		n % 2 == 0 ? exp(x * x, n / 2) :
		exp(x * x, (n - 1) / 2) * x;
}

// Pass by reference 
// 'const' keywords is necessary
static constexpr float exp2(const float &x, const int &n)
{
	return n == 0 ? 1 :
		n % 2 == 0 ? exp(x * x, n / 2) :
		exp(x * x, (n - 1) / 2) * x;
}

// Compile time computation of array length
// 'static' 'const' is optional here
template<typename T, int N>
static const int length(const T(&ary)[N])
{
	return N;
}

// Recursive constexpr function
constexpr int factorial(unsigned int n)
{
	return n == 0 ? 1 :
		n == 1 ? 1 : n * factorial(n - 1);
}

// User-defined type
class Foo
{
	public:
		constexpr explicit Foo(int i) : _i(i) {}
		// 'const' is necessary, since this is const
		constexpr int GetValue() const { return _i; }
	private:
		int _i;
};

int main()
{
	numeric_limits_test();

	// Compile time:
	constexpr float x = 42.0;
	std::cout << x << std::endl;
	constexpr float x2 = -1.0 * x;
	std::cout << x2 << std::endl;
	constexpr float y{108};
	std::cout << y << std::endl;
	constexpr float z = exp(5, 3);
	std::cout << z << std::endl;
	constexpr int three = 3;
	constexpr float a = exp2(x, three);
	std::cout << a << std::endl;
	int week[7];
	std::cout << length(week) << std::endl;
	std::cout << factorial(0) << std::endl;
	// foo is const
	constexpr Foo foo(10);
	constexpr int value = foo.GetValue();
	std::cout << value << std::endl;

	// Run time:
	std::cout << "The value of foo is " << foo.GetValue() << std::endl;


	
	return 0;
}

#define CONSTEXPR(expression) \
	std::cout << #expression << " = " << expression << std::endl

static void numeric_limits_test(void)
{
	CONSTEXPR(std::numeric_limits<double>::infinity());
	CONSTEXPR(std::numeric_limits<double>::max());
	CONSTEXPR(std::numeric_limits<double>::min());
	CONSTEXPR(std::numeric_limits<double>::lowest());
	constexpr double test = -std::numeric_limits<double>::infinity();
	std::cout << test << std::endl;
}


