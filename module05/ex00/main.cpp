#include "Bureaucrat.hpp"

int main()
{
	std::cout << "----- Valid Bureaucrat -----" << std::endl;
	try
	{
		Bureaucrat a("Alice", 42);
		std::cout << a << std::endl;

		a.incrementGrade();
		std::cout << "After increment: " << a << std::endl;

		a.decrementGrade();
		std::cout << "After decrement: " << a << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n----- Grade Too High -----" << std::endl;
	try
	{
		Bureaucrat b("Bob", 0);
		std::cout << b << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n----- Grade Too Low -----" << std::endl;
	try
	{
		Bureaucrat c("Charlie", 151);
		std::cout << c << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n----- Increment Exception -----" << std::endl;
	try
	{
		Bureaucrat d("David", 1);
		std::cout << d << std::endl;

		d.incrementGrade();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n----- Decrement Exception -----" << std::endl;
	try
	{
		Bureaucrat e("Eve", 150);
		std::cout << e << std::endl;

		e.decrementGrade();
	}
	catch (std::exception &e1)
	{
		std::cout << e1.what() << std::endl;
	}

	return (0);
}