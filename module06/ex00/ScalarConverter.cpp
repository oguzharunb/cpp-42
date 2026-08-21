#include "ScalarConverter.hpp"
#include <cerrno>
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <limits>
#include <sstream>

static bool isCharLiteral(const std::string &s)
{
	if (s.length() == 1
		&& !std::isdigit(static_cast<unsigned char>(s[0])))
		return (true);
	return (s.length() == 3 && s[0] == '\'' && s[2] == '\'');
}

static char getCharLiteral(const std::string &s)
{
	if (s.length() == 1)
		return (s[0]);
	return (s[1]);
}

static bool isDecimalLiteral(const std::string &s, bool hasFloatSuffix)
{
	std::string value = s;

	if (hasFloatSuffix)
		value.erase(value.length() - 1);
	if (value.empty())
		return (false);

	std::string::size_type i = 0;
	if (value[i] == '+' || value[i] == '-')
		i++;
	if (i == value.length())
		return (false);

	bool hasDigit = false;
	while (i < value.length()
		&& std::isdigit(static_cast<unsigned char>(value[i])))
	{
		hasDigit = true;
		i++;
	}

	bool hasDecimalPoint = false;
	if (i < value.length() && value[i] == '.')
	{
		hasDecimalPoint = true;
		i++;
		while (i < value.length()
			&& std::isdigit(static_cast<unsigned char>(value[i])))
		{
			hasDigit = true;
			i++;
		}
	}
	if (!hasDigit)
		return (false);

	bool hasExponent = false;
	if (i < value.length() && (value[i] == 'e' || value[i] == 'E'))
	{
		hasExponent = true;
		i++;
		if (i < value.length() && (value[i] == '+' || value[i] == '-'))
			i++;
		std::string::size_type exponentStart = i;
		while (i < value.length()
			&& std::isdigit(static_cast<unsigned char>(value[i])))
			i++;
		if (i == exponentStart)
			return (false);
	}
	if (i != value.length())
		return (false);
	if (hasFloatSuffix && !hasDecimalPoint && !hasExponent)
		return (false);
	return (true);
}

static bool isNan(double value)
{
	return (value != value);
}

static bool isInfinite(double value)
{
	return (!isNan(value)
		&& (value == std::numeric_limits<double>::infinity()
			|| value == -std::numeric_limits<double>::infinity()));
}

static std::string formatFinite(double value, int precision)
{
	std::ostringstream output;

	output << std::setprecision(precision) << value;
	std::string result = output.str();
	if (result.find('.') == std::string::npos
		&& result.find('e') == std::string::npos
		&& result.find('E') == std::string::npos)
		result += ".0";
	return (result);
}

static void printChar(double value)
{
	if (isNan(value) || isInfinite(value)
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
	if (isNan(value)
		|| isInfinite(value)
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

static void printFloat(double value, bool overflowed)
{
	std::cout << "float: ";

	if (overflowed
		|| (!isInfinite(value)
			&& (value > std::numeric_limits<float>::max()
				|| value < -std::numeric_limits<float>::max())))
	{
		std::cout << "impossible";
	}
	else if (isNan(value))
	{
		std::cout << "nanf";
	}
	else if (isInfinite(value))
	{
		if (value > 0)
			std::cout << "+inff";
		else
			std::cout << "-inff";
	}
	else
	{
		std::cout << formatFinite(static_cast<float>(value),
								 std::numeric_limits<float>::digits10 + 1)
				  << "f";
	}

	std::cout << std::endl;
}

static void printDouble(double value, bool overflowed)
{
	std::cout << "double: ";

	if (overflowed)
	{
		std::cout << "impossible";
	}
	else if (isNan(value))
	{
		std::cout << "nan";
	}
	else if (isInfinite(value))
	{
		if (value > 0)
			std::cout << "+inf";
		else
			std::cout << "-inf";
	}
	else
	{
		std::cout << formatFinite(value,
								 std::numeric_limits<double>::digits10 + 1);
	}

	std::cout << std::endl;
}

void ScalarConverter::convert(const std::string &literal)
{
	double value;
	bool overflowed = false;

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
		value = static_cast<double>(getCharLiteral(literal));
	}
	else
	{
		bool hasFloatSuffix = literal.length() > 1
			&& literal[literal.length() - 1] == 'f';
		if (!isDecimalLiteral(literal, hasFloatSuffix))
		{
			std::cout << "Invalid literal" << std::endl;
			return;
		}

		std::string numericPart = literal;
		if (hasFloatSuffix)
			numericPart.erase(numericPart.length() - 1);
		char *end;

		errno = 0;
		value = std::strtod(numericPart.c_str(), &end);
		if (end == numericPart.c_str() || *end != '\0')
		{
			std::cout << "Invalid literal" << std::endl;
			return;
		}
		overflowed = errno == ERANGE && isInfinite(value);
	}

	printChar(value);
	printInt(value);
	printFloat(value, overflowed);
	printDouble(value, overflowed);
}
