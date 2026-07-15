#include "ScalarConverter.hpp"
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <cmath>

static bool isCharLiteral(const std::string &s)
{
	return (s.length() == 3 && s[0] == '\'' && s[2] == '\'');
}

static bool isFloatLiteral(const std::string &s)
{
	return (!s.empty() && s[s.length() - 1] == 'f');
}

static void printChar(double value)
{
	if (std::isnan(value) || std::isinf(value)
		|| value < 0 || value > 127)
	{
		std::cout << "char: impossible" << std::endl;
	}
	else if (!std::isprint(static_cast<int>(value)))
	{
		std::cout << "char: Non displayable" << std::endl;
	}
	else
	{
		std::cout << "char: '"
				  << static_cast<char>(value)
				  << "'"
				  << std::endl;
	}
}

static void printInt(double value)
{
	if (std::isnan(value)
		|| std::isinf(value)
		|| value > std::numeric_limits<int>::max()
		|| value < std::numeric_limits<int>::min())
	{
		std::cout << "int: impossible" << std::endl;
	}
	else
	{
		std::cout << "int: "
				  << static_cast<int>(value)
				  << std::endl;
	}
}

static void printFloat(double value)
{
	std::cout << "float: ";

	if (std::isnan(value))
	{
		std::cout << "nanf";
	}
	else if (std::isinf(value))
	{
		if (value > 0)
			std::cout << "+inff";
		else
			std::cout << "-inff";
	}
	else
	{
		std::cout << std::fixed
				  << std::setprecision(1)
				  << static_cast<float>(value)
				  << "f";
	}

	std::cout << std::endl;
}

static void printDouble(double value)
{
	std::cout << "double: ";

	if (std::isnan(value))
	{
		std::cout << "nan";
	}
	else if (std::isinf(value))
	{
		if (value > 0)
			std::cout << "+inf";
		else
			std::cout << "-inf";
	}
	else
	{
		std::cout << std::fixed
				  << std::setprecision(1)
				  << value;
	}

	std::cout << std::endl;
}

void ScalarConverter::convert(const std::string &literal)
{
	double value;

	if (literal == "nan" || literal == "nanf")
	{
		value = std::numeric_limits<double>::quiet_NaN();
	}
	else if (literal == "+inf"
		|| literal == "+inff")
	{
		value = std::numeric_limits<double>::infinity();
	}
	else if (literal == "-inf"
		|| literal == "-inff")
	{
		value = -std::numeric_limits<double>::infinity();
	}
	else if (isCharLiteral(literal))
	{
		value = static_cast<double>(literal[1]);
	}
	else
	{
		char *end;

		value = std::strtod(literal.c_str(), &end);

		if (*end == 'f' && *(end + 1) == '\0')
		{
			// float literal, already converted by strtod
		}
		else if (*end != '\0')
		{
			std::cout << "Invalid literal" << std::endl;
			return;
		}
	}

	printChar(value);
	printInt(value);
	printFloat(value);
	printDouble(value);
}
