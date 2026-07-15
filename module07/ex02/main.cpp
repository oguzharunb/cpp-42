#include "Array.hpp"

#include <iostream>
#include <string>

int main()
{
	std::cout << "===== INT ARRAY =====" << std::endl;

	Array<int> numbers(5);

	for (unsigned int i = 0; i < numbers.size(); i++)
	{
		numbers[i] = i * 10;
	}

	for (unsigned int i = 0; i < numbers.size(); i++)
	{
		std::cout << "numbers[" << i << "] = "
				  << numbers[i]
				  << std::endl;
	}


	std::cout << "\n===== COPY CONSTRUCTOR =====" << std::endl;

	Array<int> copy(numbers);

	copy[0] = 999;

	std::cout << "Original first element: "
			  << numbers[0]
			  << std::endl;

	std::cout << "Copy first element: "
			  << copy[0]
			  << std::endl;


	std::cout << "\n===== ASSIGNMENT OPERATOR =====" << std::endl;

	Array<int> assigned;

	assigned = numbers;

	assigned[1] = 555;

	std::cout << "Original second element: "
			  << numbers[1]
			  << std::endl;

	std::cout << "Assigned second element: "
			  << assigned[1]
			  << std::endl;


	std::cout << "\n===== STRING ARRAY =====" << std::endl;

	Array<std::string> words(3);

	words[0] = "hello";
	words[1] = "42";
	words[2] = "cpp";

	for (unsigned int i = 0; i < words.size(); i++)
	{
		std::cout << words[i]
				  << std::endl;
	}


	std::cout << "\n===== EXCEPTION TEST =====" << std::endl;

	try
	{
		std::cout << numbers[10]
				  << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << "Exception caught: "
				  << e.what()
				  << std::endl;
	}


	std::cout << "\n===== EMPTY ARRAY =====" << std::endl;

	Array<int> empty;

	std::cout << "Size: "
			  << empty.size()
			  << std::endl;

	return (0);
}
