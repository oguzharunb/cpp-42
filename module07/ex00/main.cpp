#include "whatever.hpp"
#include <iostream>
#include <string>

int main(void)
{
	std::cout << "===== INT TEST =====" << std::endl;

	int a = 2;
	int b = 3;

	std::cout << "Before swap:" << std::endl;
	std::cout << "a = " << a << ", b = " << b << std::endl;

	::swap(a, b);

	std::cout << "After swap:" << std::endl;
	std::cout << "a = " << a << ", b = " << b << std::endl;

	std::cout << "min(a, b) = "
			  << ::min(a, b)
			  << std::endl;

	std::cout << "max(a, b) = "
			  << ::max(a, b)
			  << std::endl;

	std::cout << "\n===== STRING TEST =====" << std::endl;

	std::string c = "chaine1";
	std::string d = "chaine2";

	std::cout << "Before swap:" << std::endl;
	std::cout << "c = " << c
			  << ", d = " << d
			  << std::endl;

	::swap(c, d);

	std::cout << "After swap:" << std::endl;
	std::cout << "c = " << c
			  << ", d = " << d
			  << std::endl;

	std::cout << "min(c, d) = "
			  << ::min(c, d)
			  << std::endl;

	std::cout << "max(c, d) = "
			  << ::max(c, d)
			  << std::endl;

	std::cout << "\n===== EQUAL VALUES TEST =====" << std::endl;

	int x = 42;
	int y = 42;

	std::cout << "min(x, y) = "
			  << ::min(x, y)
			  << std::endl;

	std::cout << "max(x, y) = "
			  << ::max(x, y)
			  << std::endl;

	std::cout << "min returns second: "
			  << (&::min(x, y) == &y ? "Yes" : "No")
			  << std::endl;
	std::cout << "max returns second: "
			  << (&::max(x, y) == &y ? "Yes" : "No")
			  << std::endl;

	std::cout << "\n===== CONST VALUES TEST =====" << std::endl;

	const int low = 7;
	const int high = 9;

	std::cout << "min(low, high) = " << ::min(low, high) << std::endl;
	std::cout << "max(low, high) = " << ::max(low, high) << std::endl;

	return (0);
}
