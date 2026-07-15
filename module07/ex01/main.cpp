#include "iter.hpp"

#include <iostream>
#include <string>

template <typename T>
void print(T &value)
{
	std::cout << value << std::endl;
}

template <typename T>
void printConst(const T &value)
{
	std::cout << value << std::endl;
}

template <typename T>
void doubleValue(T &value)
{
	value *= 2;
}

int main()
{
	std::cout << "===== INT ARRAY =====" << std::endl;

	int numbers[] = {1, 2, 3, 4, 5};

	std::cout << "Before:" << std::endl;
	iter(numbers, 5, print<int>);

	iter(numbers, 5, doubleValue<int>);

	std::cout << "After multiplying by 2:" << std::endl;
	iter(numbers, 5, print<int>);


	std::cout << "\n===== STRING ARRAY =====" << std::endl;

	std::string words[] =
	{
		"hello",
		"42",
		"cpp"
	};

	iter(words, 3, print<std::string>);


	std::cout << "\n===== CONST ARRAY =====" << std::endl;

	const double values[] =
	{
		1.5,
		2.5,
		3.5
	};

	iter(values, 3, printConst<double>);


	return (0);
}
