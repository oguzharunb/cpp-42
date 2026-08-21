#include "Base.hpp"
#include "A.hpp"
#include "B.hpp"
#include "C.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

Base *generate(void)
{
	int random = rand() % 3;

	if (random == 0)
		return (new A());
	else if (random == 1)
		return (new B());
	else
		return (new C());
}

void identify(Base *p)
{
	if (dynamic_cast<A *>(p))
		std::cout << "A" << std::endl;
	else if (dynamic_cast<B *>(p))
		std::cout << "B" << std::endl;
	else if (dynamic_cast<C *>(p))
		std::cout << "C" << std::endl;
	else
		std::cout << "Unknown type" << std::endl;
}

void identify(Base &p)
{
	try
	{
		(void)dynamic_cast<A &>(p);
		std::cout << "A" << std::endl;
		return;
	}
	catch (const std::exception &)
	{
	}

	try
	{
		(void)dynamic_cast<B &>(p);
		std::cout << "B" << std::endl;
		return;
	}
	catch (const std::exception &)
	{
	}

	try
	{
		(void)dynamic_cast<C &>(p);
		std::cout << "C" << std::endl;
		return;
	}
	catch (const std::exception &)
	{
	}

	std::cout << "Unknown type" << std::endl;
}

int main()
{
	srand(time(NULL));

	std::cout << "===== Generated object =====" << std::endl;

	Base *object = generate();

	std::cout << "Pointer: ";
	identify(object);
	std::cout << "Reference: ";
	identify(*object);

	delete object;

	std::cout << "\n===== Deterministic tests =====" << std::endl;

	A a;
	B b;
	C c;

	std::cout << "A pointer: ";
	identify(&a);
	std::cout << "A reference: ";
	identify(a);
	std::cout << "B pointer: ";
	identify(&b);
	std::cout << "B reference: ";
	identify(b);
	std::cout << "C pointer: ";
	identify(&c);
	std::cout << "C reference: ";
	identify(c);

	return (0);
}
