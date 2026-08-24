#include "MutantStack.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <stack>

static bool subjectTest()
{
	MutantStack<int> numbers;

	numbers.push(5);
	numbers.push(17);
	std::cout << "top before pop: " << numbers.top() << std::endl;
	numbers.pop();
	std::cout << "size after pop: " << numbers.size() << std::endl;
	numbers.push(3);
	numbers.push(5);
	numbers.push(737);
	numbers.push(0);
	std::cout << "forward:";
	for (MutantStack<int>::iterator it = numbers.begin();
		it != numbers.end(); ++it)
		std::cout << " " << *it;
	std::cout << std::endl;
	return (numbers.top() == 0 && numbers.size() == 5);
}

static bool copyAndStackTest()
{
	MutantStack<int> original;

	original.push(10);
	original.push(20);
	MutantStack<int> copy(original);
	copy.top() = 99;
	std::stack<int> regularStack(copy);
	bool passed = original.top() == 20
		&& copy.top() == 99
		&& regularStack.top() == 99;
	std::cout << (passed ? "[OK]   " : "[FAIL] ")
		<< "copy and std::stack conversion" << std::endl;
	return (passed);
}

static bool constAndReverseTest()
{
	MutantStack<int> numbers;

	numbers.push(1);
	numbers.push(2);
	numbers.push(3);
	const MutantStack<int> constNumbers(numbers);
	MutantStack<int>::const_iterator first = constNumbers.begin();
	MutantStack<int>::const_reverse_iterator last = constNumbers.rbegin();
	bool passed = *first == 1 && *last == 3;
	std::cout << (passed ? "[OK]   " : "[FAIL] ")
		<< "const and reverse iterators" << std::endl;
	return (passed);
}

static bool alternateContainerTest()
{
	MutantStack<int, std::list<int> > numbers;

	numbers.push(7);
	numbers.push(8);
	numbers.push(9);
	int sum = 0;
	for (MutantStack<int, std::list<int> >::iterator it = numbers.begin();
		it != numbers.end(); ++it)
		sum += *it;
	bool passed = sum == 24 && numbers.top() == 9;
	std::cout << (passed ? "[OK]   " : "[FAIL] ")
		<< "alternate underlying container" << std::endl;
	return (passed);
}

int main()
{
	bool passed = true;

	std::cout << "===== MUTANT STACK =====" << std::endl;
	passed = subjectTest() && passed;
	passed = copyAndStackTest() && passed;
	passed = constAndReverseTest() && passed;
	passed = alternateContainerTest() && passed;
	std::cout << (passed ? "All tests passed." : "Some tests failed.")
		<< std::endl;
	return (passed ? EXIT_SUCCESS : EXIT_FAILURE);
}
