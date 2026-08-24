#include "Span.hpp"

#include <climits>
#include <cstdlib>
#include <iostream>
#include <vector>

static bool check(const char *name, unsigned int actual, unsigned int expected)
{
	bool passed = actual == expected;

	std::cout << (passed ? "[OK]   " : "[FAIL] ") << name
		<< ": expected " << expected << ", got " << actual << std::endl;
	return (passed);
}

static bool subjectTest()
{
	Span span(5);

	span.addNumber(6);
	span.addNumber(3);
	span.addNumber(17);
	span.addNumber(9);
	span.addNumber(11);
	return (check("subject shortest", span.shortestSpan(), 2)
		&& check("subject longest", span.longestSpan(), 14));
}

static bool exceptionTests()
{
	bool passed = true;
	Span empty(1);

	try
	{
		empty.shortestSpan();
		passed = false;
	}
	catch (const Span::NoSpanException &)
	{
		std::cout << "[OK]   empty Span throws" << std::endl;
	}
	empty.addNumber(42);
	try
	{
		empty.longestSpan();
		passed = false;
	}
	catch (const Span::NoSpanException &)
	{
		std::cout << "[OK]   one-number Span throws" << std::endl;
	}
	try
	{
		empty.addNumber(43);
		passed = false;
	}
	catch (const Span::FullSpanException &)
	{
		std::cout << "[OK]   full Span rejects a number" << std::endl;
	}
	return (passed);
}

static bool rangeTest()
{
	const unsigned int count = 20000;
	std::vector<int> source;
	Span span(count);

	for (unsigned int i = 0; i < count; ++i)
		source.push_back(static_cast<int>(i * 3));
	span.addNumber(source.begin(), source.end());
	return (check("20,000-number shortest", span.shortestSpan(), 3)
		&& check("20,000-number longest", span.longestSpan(),
			(count - 1) * 3));
}

static bool rangeOverflowTest()
{
	int initial[] = {1, 2};
	int tooMany[] = {3, 4};
	Span span(3);

	span.addNumber(initial, initial + 2);
	try
	{
		span.addNumber(tooMany, tooMany + 2);
	}
	catch (const Span::FullSpanException &)
	{
		span.addNumber(9);
		std::cout << "[OK]   failed range insertion changes nothing"
			<< std::endl;
		return (check("range rollback longest", span.longestSpan(), 8));
	}
	std::cout << "[FAIL] oversized range did not throw" << std::endl;
	return (false);
}

static bool integerLimitsTest()
{
	Span span(3);

	span.addNumber(INT_MIN);
	span.addNumber(0);
	span.addNumber(INT_MAX);
	return (check("integer limits shortest", span.shortestSpan(),
		static_cast<unsigned int>(INT_MAX))
		&& check("integer limits longest", span.longestSpan(), UINT_MAX));
}

int main()
{
	bool passed = true;

	std::cout << "===== SPAN =====" << std::endl;
	passed = subjectTest() && passed;
	passed = exceptionTests() && passed;
	passed = rangeTest() && passed;
	passed = rangeOverflowTest() && passed;
	passed = integerLimitsTest() && passed;
	std::cout << (passed ? "All tests passed." : "Some tests failed.")
		<< std::endl;
	return (passed ? EXIT_SUCCESS : EXIT_FAILURE);
}
