#include "BitcoinExchange.hpp"

#include <exception>
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }
    try
    {
        BitcoinExchange exchange("data.csv");
        exchange.processFile(argv[1]);
    }
    catch (const std::exception &exception)
    {
        std::cerr << "Error: " << exception.what() << std::endl;
        return 1;
    }
    return 0;
}
