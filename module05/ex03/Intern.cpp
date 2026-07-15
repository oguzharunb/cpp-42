#include "Intern.hpp"
#include "AForm.hpp"
#include "ShrubberyCreationForm.hpp"
#include "RobotomyRequestForm.hpp"
#include "PresidentialPardonForm.hpp"

Intern::Intern()
{
}

Intern::Intern(const Intern &other)
{
	(void)other;
}

Intern &Intern::operator=(const Intern &other)
{
	(void)other;
	return (*this);
}

Intern::~Intern()
{
}

AForm *Intern::createShrubbery(const std::string &target) const
{
	return (new ShrubberyCreationForm(target));
}

AForm *Intern::createRobotomy(const std::string &target) const
{
	return (new RobotomyRequestForm(target));
}

AForm *Intern::createPresidential(const std::string &target) const
{
	return (new PresidentialPardonForm(target));
}

AForm *Intern::makeForm(const std::string &formName,
						const std::string &target) const
{
	typedef AForm *(Intern::*Creator)(const std::string &) const;

	struct FormFactory
	{
		std::string name;
		Creator creator;
	};

	FormFactory forms[] =
	{
		{"shrubbery creation", &Intern::createShrubbery},
		{"robotomy request", &Intern::createRobotomy},
		{"presidential pardon", &Intern::createPresidential}
	};

	for (int i = 0; i < 3; i++)
	{
		if (forms[i].name == formName)
		{
			std::cout << "Intern creates "
					  << formName
					  << std::endl;

			return ((this->*forms[i].creator)(target));
		}
	}

	std::cout << "Intern couldn't create "
			  << formName
			  << ": form does not exist."
			  << std::endl;

	return (NULL);
}
