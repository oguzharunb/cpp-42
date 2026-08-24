#ifndef SPAN_HPP
# define SPAN_HPP

# include <exception>
# include <vector>

class Span
{
	private:
		unsigned int		_capacity;
		std::vector<int>	_numbers;

	public:
		explicit Span(unsigned int capacity);
		Span(const Span &other);
		Span &operator=(const Span &other);
		~Span();

		void addNumber(int number);

		template <typename InputIterator>
		void addNumber(InputIterator first, InputIterator last)
		{
			std::vector<int> pending;

			while (first != last)
			{
				if (pending.size() >= _capacity - _numbers.size())
					throw FullSpanException();
				pending.push_back(*first);
				++first;
			}
			_numbers.insert(_numbers.end(), pending.begin(), pending.end());
		}

		unsigned int shortestSpan() const;
		unsigned int longestSpan() const;

		class FullSpanException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};

		class NoSpanException : public std::exception
		{
			public:
				virtual const char *what() const throw();
		};
};

#endif
