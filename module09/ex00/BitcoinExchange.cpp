#include "BitcoinExchange.hpp"

#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>

BitcoinExchange::BitcoinExchange()
{
    loadDatabase("data.csv");
}

BitcoinExchange::BitcoinExchange(const std::string &databasePath)
{
    loadDatabase(databasePath);
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : _rates(other._rates)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
    if (this != &other)
        _rates = other._rates;
    return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

std::string BitcoinExchange::trim(const std::string &text)
{
    const std::string whitespace = " \t\r\n";
    const std::string::size_type first = text.find_first_not_of(whitespace);

    if (first == std::string::npos)
        return "";
    return text.substr(first, text.find_last_not_of(whitespace) - first + 1);
}

bool BitcoinExchange::isValidDate(const std::string &date)
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;
    for (std::string::size_type i = 0; i < date.size(); ++i)
    {
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9'))
            return false;
    }
    const int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100
        + (date[2] - '0') * 10 + date[3] - '0';
    const int month = (date[5] - '0') * 10 + date[6] - '0';
    const int day = (date[8] - '0') * 10 + date[9] - '0';
    static const int daysPerMonth[12] =
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (year == 0 || month < 1 || month > 12 || day < 1)
        return false;
    int maxDay = daysPerMonth[month - 1];
    const bool leap = year % 400 == 0 || (year % 4 == 0 && year % 100 != 0);
    if (month == 2 && leap)
        ++maxDay;
    return day <= maxDay;
}

bool BitcoinExchange::parseNumber(const std::string &text, double &value)
{
    if (text.empty())
        return false;
    std::string::size_type position = 0;
    if (text[position] == '+' || text[position] == '-')
        ++position;
    bool hasDigits = false;
    while (position < text.size() && text[position] >= '0'
        && text[position] <= '9')
    {
        hasDigits = true;
        ++position;
    }
    if (position < text.size() && text[position] == '.')
    {
        ++position;
        while (position < text.size() && text[position] >= '0'
            && text[position] <= '9')
        {
            hasDigits = true;
            ++position;
        }
    }
    if (!hasDigits)
        return false;
    if (position < text.size()
        && (text[position] == 'e' || text[position] == 'E'))
    {
        ++position;
        if (position < text.size()
            && (text[position] == '+' || text[position] == '-'))
            ++position;
        const std::string::size_type exponentStart = position;
        while (position < text.size() && text[position] >= '0'
            && text[position] <= '9')
            ++position;
        if (position == exponentStart)
            return false;
    }
    if (position != text.size())
        return false;
    char *end = NULL;
    errno = 0;
    value = std::strtod(text.c_str(), &end);
    if (end == text.c_str() || *end != '\0' || errno == ERANGE)
        return false;
    if (value != value || value > std::numeric_limits<double>::max()
        || value < -std::numeric_limits<double>::max())
        return false;
    return true;
}

void BitcoinExchange::loadDatabase(const std::string &databasePath)
{
    std::ifstream file(databasePath.c_str());
    if (!file.is_open())
        throw std::runtime_error("could not open database.");

    std::string line;
    if (!std::getline(file, line) || trim(line) != "date,exchange_rate")
        throw std::runtime_error("invalid database.");
    while (std::getline(file, line))
    {
        if (trim(line).empty())
            continue;
        const std::string::size_type comma = line.find(',');
        if (comma == std::string::npos || comma != line.rfind(','))
            throw std::runtime_error("invalid database.");
        const std::string date = trim(line.substr(0, comma));
        const std::string rateText = trim(line.substr(comma + 1));
        double rate = 0.0;
        if (!isValidDate(date) || !parseNumber(rateText, rate) || rate < 0.0)
            throw std::runtime_error("invalid database.");
        _rates[date] = rate;
    }
    if (_rates.empty())
        throw std::runtime_error("invalid database.");
}

bool BitcoinExchange::findRate(const std::string &date, double &rate) const
{
    std::map<std::string, double>::const_iterator it = _rates.lower_bound(date);

    if (it != _rates.end() && it->first == date)
    {
        rate = it->second;
        return true;
    }
    if (it == _rates.begin())
        return false;
    --it;
    rate = it->second;
    return true;
}

void BitcoinExchange::processFile(const std::string &inputPath) const
{
    std::ifstream file(inputPath.c_str());
    if (!file.is_open())
        throw std::runtime_error("could not open file.");

    std::string line;
    if (!std::getline(file, line) || trim(line) != "date | value")
        throw std::runtime_error("bad input file.");
    while (std::getline(file, line))
    {
        const std::string original = trim(line);
        const std::string::size_type separator = original.find('|');
        if (separator == std::string::npos
            || separator != original.rfind('|'))
        {
            std::cerr << "Error: bad input => " << original << std::endl;
            continue;
        }

        const std::string date = trim(original.substr(0, separator));
        const std::string valueText = trim(original.substr(separator + 1));
        double value = 0.0;
        if (!isValidDate(date) || !parseNumber(valueText, value))
        {
            std::cerr << "Error: bad input => " << original << std::endl;
            continue;
        }
        if (value < 0.0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000.0)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        double rate = 0.0;
        if (!findRate(date, rate))
        {
            std::cerr << "Error: no exchange rate available for " << date
                      << '.' << std::endl;
            continue;
        }
        std::cout << date << " => " << value << " = " << value * rate
                  << std::endl;
    }
}
