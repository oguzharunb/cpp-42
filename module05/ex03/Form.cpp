#include "Form.hpp"
#include "Bureaucrat.hpp"

Form::Form()
	: _name("Default"), _isSigned(false), _signGrade(150), _execGrade(150)
{
}

Form::Form(const std::string &name, int signGrade, int execGrade)
	: _name(name), _isSigned(false),
	  _signGrade(signGrade), _execGrade(execGrade)
{
	if (_signGrade < 1 || _execGrade < 1)
		throw GradeTooHighException();
	if (_signGrade > 150 || _execGrade > 150)
		throw GradeTooLowException();
}

Form::Form(const Form &other)
	: _name(other._name),
	  _isSigned(other._isSigned),
	  _signGrade(other._signGrade),
	  _execGrade(other._execGrade)
{
}

Form &Form::operator=(const Form &other)
{
	if (this != &other)
		_isSigned = other._isSigned;
	return (*this);
}

Form::~Form()
{
}

const std::string &Form::getName() const
{
	return (_name);
}

bool Form::getIsSigned() const
{
	return (_isSigned);
}

int Form::getSignGrade() const
{
	return (_signGrade);
}

int Form::getExecGrade() const
{
	return (_execGrade);
}

void Form::beSigned(const Bureaucrat &bureaucrat)
{
	if (bureaucrat.getGrade() > _signGrade)
		throw GradeTooLowException();

	_isSigned = true;
}

const char *Form::GradeTooHighException::what() const throw()
{
	return ("Form grade is too high.");
}

const char *Form::GradeTooLowException::what() const throw()
{
	return ("Form grade is too low.");
}

std::ostream &operator<<(std::ostream &out, const Form &form)
{
	out << "Form \"" << form.getName() << "\""
		<< " | Signed: " << (form.getIsSigned() ? "Yes" : "No")
		<< " | Sign Grade: " << form.getSignGrade()
		<< " | Execute Grade: " << form.getExecGrade();

	return (out);
}
