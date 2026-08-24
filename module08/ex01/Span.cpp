#include "Span.hpp"

#include <algorithm>
#include <climits>

Span::Span(unsigned int capacity)
	: _capacity(capacity)
{
}

Span::Span(const Span &other)
	: _capacity(other._capacity), _numbers(other._numbers)
{
}

Span &Span::operator=(const Span &other)
{
	if (this != &other)
	{
		std::vector<int> copy(other._numbers);

		_numbers.swap(copy);
		_capacity = other._capacity;
	}
	return (*this);
}

Span::~Span()
{
}

void Span::addNumber(int number)
{
	if (_numbers.size() >= _capacity)
		throw FullSpanException();
	_numbers.push_back(number);
}

unsigned int Span::shortestSpan() const
{
	std::vector<int> sorted;
	unsigned int shortest;

	if (_numbers.size() < 2)
		throw NoSpanException();
	sorted = _numbers;
	std::sort(sorted.begin(), sorted.end());
	shortest = UINT_MAX;
	for (std::vector<int>::size_type i = 1; i < sorted.size(); ++i)
	{
		unsigned int current = static_cast<unsigned int>(sorted[i])
			- static_cast<unsigned int>(sorted[i - 1]);
		if (current < shortest)
			shortest = current;
	}
	return (shortest);
}

unsigned int Span::longestSpan() const
{
	std::vector<int>::const_iterator smallest;
	std::vector<int>::const_iterator largest;

	if (_numbers.size() < 2)
		throw NoSpanException();
	smallest = std::min_element(_numbers.begin(), _numbers.end());
	largest = std::max_element(_numbers.begin(), _numbers.end());
	return (static_cast<unsigned int>(*largest)
		- static_cast<unsigned int>(*smallest));
}

const char *Span::FullSpanException::what() const throw()
{
	return ("Span is full");
}

const char *Span::NoSpanException::what() const throw()
{
	return ("Not enough numbers to calculate a span");
}
