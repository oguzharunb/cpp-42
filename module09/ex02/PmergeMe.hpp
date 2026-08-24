#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <deque>
# include <string>
# include <vector>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void run(int argc, char **argv);

private:
    struct VectorElement
    {
        int value;
        unsigned long id;
        VectorElement();
        VectorElement(int number, unsigned long identifier);
    };

    struct VectorPair
    {
        VectorElement low;
        unsigned long highId;
        VectorPair();
        VectorPair(const VectorElement &element, unsigned long identifier);
    };

    struct DequeElement
    {
        int value;
        unsigned long id;
        DequeElement();
        DequeElement(int number, unsigned long identifier);
    };

    struct DequePair
    {
        DequeElement low;
        unsigned long highId;
        DequePair();
        DequePair(const DequeElement &element, unsigned long identifier);
    };

    std::vector<int> _before;
    std::vector<int> _vectorResult;
    std::deque<int> _dequeResult;
    double _vectorTime;
    double _dequeTime;

    static int parsePositiveInteger(const std::string &text);
    static std::vector<int> sortVector(const std::vector<int> &values);
    static std::deque<int> sortDeque(const std::deque<int> &values);
    static std::vector<VectorElement> fordJohnsonVector(
        const std::vector<VectorElement> &values);
    static std::deque<DequeElement> fordJohnsonDeque(
        const std::deque<DequeElement> &values);
    static void insertVectorPending(std::vector<VectorElement> &chain,
        const VectorPair &pending);
    static void insertDequePending(std::deque<DequeElement> &chain,
        const DequePair &pending);
    static void printSequence(const std::string &label,
        const std::vector<int> &values);
    void verifyResults() const;
};

#endif
