#ifndef MUTANTSTACK_HPP
# define MUTANTSTACK_HPP

# include <deque>
# include <stack>

template <typename T, typename Container = std::deque<T> >
class MutantStack : public std::stack<T, Container>
{
	public:
		typedef std::stack<T, Container> base_type;
		typedef Container container_type;
		typedef typename container_type::iterator iterator;
		typedef typename container_type::const_iterator const_iterator;
		typedef typename container_type::reverse_iterator reverse_iterator;
		typedef typename container_type::const_reverse_iterator const_reverse_iterator;

		MutantStack()
			: base_type()
		{
		}

		explicit MutantStack(const container_type &container)
			: base_type(container)
		{
		}

		MutantStack(const MutantStack &other)
			: base_type(other)
		{
		}

		MutantStack &operator=(const MutantStack &other)
		{
			if (this != &other)
				base_type::operator=(other);
			return (*this);
		}

		~MutantStack()
		{
		}

		iterator begin()
		{
			return (this->c.begin());
		}

		iterator end()
		{
			return (this->c.end());
		}

		const_iterator begin() const
		{
			return (this->c.begin());
		}

		const_iterator end() const
		{
			return (this->c.end());
		}

		reverse_iterator rbegin()
		{
			return (this->c.rbegin());
		}

		reverse_iterator rend()
		{
			return (this->c.rend());
		}

		const_reverse_iterator rbegin() const
		{
			return (this->c.rbegin());
		}

		const_reverse_iterator rend() const
		{
			return (this->c.rend());
		}
};

#endif
