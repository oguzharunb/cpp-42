#include "ShrubberyCreationForm.hpp"
#include <fstream>

ShrubberyCreationForm::ShrubberyCreationForm(const std::string &target)
	: AForm("ShrubberyCreationForm", 145, 137),
	  _target(target)
{
}

ShrubberyCreationForm::ShrubberyCreationForm(const ShrubberyCreationForm &other)
	: AForm(other),
	  _target(other._target)
{
}

ShrubberyCreationForm &ShrubberyCreationForm::operator=(const ShrubberyCreationForm &other)
{
	if (this != &other)
	{
		AForm::operator=(other);
	}

	return (*this);
}

ShrubberyCreationForm::~ShrubberyCreationForm()
{
}

void ShrubberyCreationForm::executeAction() const
{
	std::string filename = _target + "_shrubbery";

	std::ofstream file(filename.c_str());

	if (file.is_open())
	{
		file << "          _-_" << std::endl;
		file << "       /~~   ~~\\" << std::endl;
		file << "    /~~         ~~\\" << std::endl;
		file << "   {               }" << std::endl;
		file << "    \\  _-     -_  /" << std::endl;
		file << "      ~  \\\\ //  ~" << std::endl;
		file << "          | |" << std::endl;
		file << "          | |" << std::endl;
		file.close();
	}
}
