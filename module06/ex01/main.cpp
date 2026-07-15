#include "Serializer.hpp"
#include "Data.hpp"

int main()
{
	Data data;

	data.name = "Bender";
	data.number = 42;

	std::cout << "Original Data:" << std::endl;
	std::cout << "Name: "
			  << data.name
			  << std::endl;
	std::cout << "Number: "
			  << data.number
			  << std::endl;

	std::cout << "\n--- Serialization ---" << std::endl;

	Data *originalPtr = &data;

	uintptr_t raw = Serializer::serialize(originalPtr);

	std::cout << "Serialized address: "
			  << raw
			  << std::endl;

	std::cout << "\n--- Deserialization ---" << std::endl;

	Data *newPtr = Serializer::deserialize(raw);

	std::cout << "Deserialized pointer address: "
			  << newPtr
			  << std::endl;


	std::cout << "\n--- Pointer Comparison ---" << std::endl;

	if (originalPtr == newPtr)
	{
		std::cout << "Success: pointers are equal."
				  << std::endl;
	}
	else
	{
		std::cout << "Error: pointers are different."
				  << std::endl;
	}

	std::cout << "\nData through deserialized pointer:"
			  << std::endl;

	std::cout << "Name: "
			  << newPtr->name
			  << std::endl;

	std::cout << "Number: "
			  << newPtr->number
			  << std::endl;

	return (0);
}
