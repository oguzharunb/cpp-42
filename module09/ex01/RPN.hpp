#ifndef RPN_HPP
# define RPN_HPP

# include <list>
# include <stack>
# include <string>

class RPN
{
public:
    RPN();
    RPN(const RPN &other);
    RPN &operator=(const RPN &other);
    ~RPN();

    long evaluate(const std::string &expression);

private:
    std::stack<long, std::list<long> > _operands;

    static bool isOperator(const std::string &token);
    void applyOperator(char operation);
};

#endif
