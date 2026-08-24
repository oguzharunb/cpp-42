#include "easyfind.hpp"

#include <deque>
#include <iostream>
#include <list>
#include <vector>

static void testVector()
{
	int values[] = {4, 8, 15, 16, 23, 42, 15};
	std::vector<int> numbers(values, values + 7);
	std::vector<int>::iterator found = easyfind(numbers, 15);

	std::cout << "vector: found " << *found
		<< " at index " << (found - numbers.begin()) << std::endl;
	*found = -15;
	std::cout << "vector: first occurrence is mutable: "
		<< numbers[2] << std::endl;
}

static void testList()
{
	std::list<int> numbers;

	numbers.push_back(10);
	numbers.push_back(20);
	numbers.push_back(30);
	std::list<int>::iterator found = easyfind(numbers, 20);
	std::cout << "list: found " << *found << std::endl;
}

static void testConstContainer()
{
	int values[] = {1, 3, 5, 7};
	const std::deque<int> numbers(values, values + 4);
	std::deque<int>::const_iterator found = easyfind(numbers, 7);

	std::cout << "const deque: found " << *found << std::endl;
}

static void testMissingValue()
{
	std::vector<int> numbers(3, 42);

	try
	{
		easyfind(numbers, 21);
		std::cout << "missing value: test failed" << std::endl;
	}
	catch (const std::exception &exception)
	{
		std::cout << "missing value: " << exception.what() << std::endl;
	}
}

static void testEmptyContainer()
{
	std::list<int> numbers;

	try
	{
		easyfind(numbers, 0);
		std::cout << "empty container: test failed" << std::endl;
	}
	catch (const std::exception &exception)
	{
		std::cout << "empty container: " << exception.what() << std::endl;
	}
}

int main()
{
	std::cout << "===== EASYFIND =====" << std::endl;
	testVector();
	testList();
	testConstContainer();
	testMissingValue();
	testEmptyContainer();
	return (0);
}
