#include "Bureaucrat.hpp"
#include "Form.hpp"

int main()
{
	std::cout << "========== VALID SIGN ==========\n";
	try
	{
		Bureaucrat alice("Alice", 10);
		Form permit("Permit", 20, 30);

		std::cout << alice << std::endl;
		std::cout << permit << std::endl;

		alice.signForm(permit);

		std::cout << permit << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n========== INVALID SIGN ==========\n";
	try
	{
		Bureaucrat bob("Bob", 100);
		Form contract("Contract", 50, 50);

		std::cout << bob << std::endl;
		std::cout << contract << std::endl;

		bob.signForm(contract);

		std::cout << contract << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n========== INVALID FORM ==========\n";
	try
	{
		Form wrong("Wrong", 0, 151);
		std::cout << wrong << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cout << "\n========== GRADE LIMITS ==========\n";
	try
	{
		Bureaucrat chief("Chief", 1);
		std::cout << chief << std::endl;

		chief.incrementGrade();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	try
	{
		Bureaucrat intern("Intern", 150);
		std::cout << intern << std::endl;

		intern.decrementGrade();
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
