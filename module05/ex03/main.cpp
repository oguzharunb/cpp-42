#include "Bureaucrat.hpp"
#include "AForm.hpp"
#include "Intern.hpp"

int main()
{
	std::cout << "========== INTERN TEST ==========" << std::endl;

	Intern someRandomIntern;

	AForm *form1;
	AForm *form2;
	AForm *form3;
	AForm *wrongForm;

	std::cout << "\n--- Creating Forms ---" << std::endl;

	form1 = someRandomIntern.makeForm(
		"shrubbery creation",
		"garden"
	);

	form2 = someRandomIntern.makeForm(
		"robotomy request",
		"Bender"
	);

	form3 = someRandomIntern.makeForm(
		"presidential pardon",
		"Arthur"
	);

	wrongForm = someRandomIntern.makeForm(
		"coffee making",
		"Machine"
	);


	std::cout << "\n--- Executing Forms ---" << std::endl;

	try
	{
		Bureaucrat boss("Boss", 1);

		if (form1)
		{
			boss.signForm(*form1);
			boss.executeForm(*form1);
		}

		if (form2)
		{
			boss.signForm(*form2);
			boss.executeForm(*form2);
		}

		if (form3)
		{
			boss.signForm(*form3);
			boss.executeForm(*form3);
		}
	}
	catch (std::exception &e)
	{
		std::cout << e.what() << std::endl;
	}


	std::cout << "\n--- Cleaning Memory ---" << std::endl;

	delete form1;
	delete form2;
	delete form3;
	delete wrongForm;

	return (0);
}
