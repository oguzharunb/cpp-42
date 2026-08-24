#ifndef EASYFIND_HPP
# define EASYFIND_HPP

# include <algorithm>
# include <exception>

class EasyFindNotFound : public std::exception
{
	public:
		virtual const char *what() const throw()
		{
			return ("easyfind: value not found");
		}
};

template <typename T>
typename T::iterator easyfind(T &container, int value)
{
	typename T::iterator result;

	result = std::find(container.begin(), container.end(), value);
	if (result == container.end())
		throw EasyFindNotFound();
	return (result);
}

template <typename T>
typename T::const_iterator easyfind(const T &container, int value)
{
	typename T::const_iterator result;

	result = std::find(container.begin(), container.end(), value);
	if (result == container.end())
		throw EasyFindNotFound();
	return (result);
}

#endif
