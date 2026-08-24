#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>

class BitcoinExchange
{
public:
    BitcoinExchange();
    explicit BitcoinExchange(const std::string &databasePath);
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);
    ~BitcoinExchange();

    void processFile(const std::string &inputPath) const;

private:
    std::map<std::string, double> _rates;

    void loadDatabase(const std::string &databasePath);
    bool findRate(const std::string &date, double &rate) const;

    static std::string trim(const std::string &text);
    static bool isValidDate(const std::string &date);
    static bool parseNumber(const std::string &text, double &value);
};

#endif
