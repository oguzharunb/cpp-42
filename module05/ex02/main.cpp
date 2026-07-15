#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

#include <cstdlib>
#include <ctime>

int main()
{
	srand(time(NULL));

	std::cout << "========== SHRUBBERY TEST ==========" << std::endl;
	try
	{
		Bureaucrat gardener("Gardener", 100);
		ShrubberyCreationForm shrub("home");

		std::cout << gardener << std::endl;
		std::cout << shrub << std::endl;

		gardener.signForm(shrub);
		gardener.executeForm(shrub);

		std::cout << shrub << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}


	std::cout << "\n========== ROBOTOMY TEST ==========" << std::endl;
	try
	{
		Bureaucrat scientist("Scientist", 40);
		RobotomyRequestForm robot("Bender");

		std::cout << scientist << std::endl;
		std::cout << robot << std::endl;

		scientist.signForm(robot);
		scientist.executeForm(robot);

		std::cout << robot << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}


	std::cout << "\n========== PRESIDENTIAL TEST ==========" << std::endl;
	try
	{
		Bureaucrat president("President", 1);
		PresidentialPardonForm pardon("Arthur");

		std::cout << president << std::endl;
		std::cout << pardon << std::endl;

		president.signForm(pardon);
		president.executeForm(pardon);

		std::cout << pardon << std::endl;
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}


	std::cout << "\n========== LOW GRADE TEST ==========" << std::endl;
	try
	{
		Bureaucrat intern("Intern", 150);
		PresidentialPardonForm pardon("Nobody");

		std::cout << intern << std::endl;

		intern.signForm(pardon);
		intern.executeForm(pardon);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}


	std::cout << "\n========== UNSIGNED FORM TEST ==========" << std::endl;
	try
	{
		Bureaucrat boss("Boss", 1);
		RobotomyRequestForm robot("Marvin");

		std::cout << boss << std::endl;
		std::cout << robot << std::endl;

		boss.executeForm(robot);
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}

	return (0);
}
