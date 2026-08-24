#include "RPN.hpp"

#include <climits>
#include <sstream>
#include <stdexcept>

RPN::RPN()
{
}

RPN::RPN(const RPN &other) : _operands(other._operands)
{
}

RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
        _operands = other._operands;
    return *this;
}

RPN::~RPN()
{
}

bool RPN::isOperator(const std::string &token)
{
    return token.size() == 1
        && (token[0] == '+' || token[0] == '-' || token[0] == '*'
            || token[0] == '/');
}

void RPN::applyOperator(char operation)
{
    if (_operands.size() < 2)
        throw std::runtime_error("Error");
    const long right = _operands.top();
    _operands.pop();
    const long left = _operands.top();
    _operands.pop();

    long result = 0;
    if (operation == '+')
    {
        if ((right > 0 && left > LONG_MAX - right)
            || (right < 0 && left < LONG_MIN - right))
            throw std::runtime_error("Error");
        result = left + right;
    }
    else if (operation == '-')
    {
        if ((right > 0 && left < LONG_MIN + right)
            || (right < 0 && left > LONG_MAX + right))
            throw std::runtime_error("Error");
        result = left - right;
    }
    else if (operation == '*')
    {
        if ((left == -1 && right == LONG_MIN)
            || (right == -1 && left == LONG_MIN)
            || (left > 0 && right > 0 && left > LONG_MAX / right)
            || (left > 0 && right < 0 && right < LONG_MIN / left)
            || (left < 0 && right > 0 && left < LONG_MIN / right)
            || (left < 0 && right < 0 && left < LONG_MAX / right))
            throw std::runtime_error("Error");
        result = left * right;
    }
    else
    {
        if (right == 0 || (left == LONG_MIN && right == -1))
            throw std::runtime_error("Error");
        result = left / right;
    }
    _operands.push(result);
}

long RPN::evaluate(const std::string &expression)
{
    while (!_operands.empty())
        _operands.pop();

    std::istringstream input(expression);
    std::string token;
    bool sawToken = false;
    while (input >> token)
    {
        sawToken = true;
        if (token.size() == 1 && token[0] >= '0' && token[0] <= '9')
            _operands.push(token[0] - '0');
        else if (isOperator(token))
            applyOperator(token[0]);
        else
            throw std::runtime_error("Error");
    }
    if (!sawToken || _operands.size() != 1)
        throw std::runtime_error("Error");
    return _operands.top();
}
