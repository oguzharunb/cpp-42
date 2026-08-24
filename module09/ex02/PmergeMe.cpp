#include "PmergeMe.hpp"

#include <climits>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <stdexcept>

PmergeMe::VectorElement::VectorElement() : value(0), id(0)
{
}

PmergeMe::VectorElement::VectorElement(int number, unsigned long identifier)
    : value(number), id(identifier)
{
}

PmergeMe::VectorPair::VectorPair() : low(), highId(0)
{
}

PmergeMe::VectorPair::VectorPair(const VectorElement &element,
    unsigned long identifier) : low(element), highId(identifier)
{
}

PmergeMe::DequeElement::DequeElement() : value(0), id(0)
{
}

PmergeMe::DequeElement::DequeElement(int number, unsigned long identifier)
    : value(number), id(identifier)
{
}

PmergeMe::DequePair::DequePair() : low(), highId(0)
{
}

PmergeMe::DequePair::DequePair(const DequeElement &element,
    unsigned long identifier) : low(element), highId(identifier)
{
}

PmergeMe::PmergeMe() : _vectorTime(0.0), _dequeTime(0.0)
{
}

PmergeMe::PmergeMe(const PmergeMe &other)
    : _before(other._before), _vectorResult(other._vectorResult),
      _dequeResult(other._dequeResult), _vectorTime(other._vectorTime),
      _dequeTime(other._dequeTime)
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _before = other._before;
        _vectorResult = other._vectorResult;
        _dequeResult = other._dequeResult;
        _vectorTime = other._vectorTime;
        _dequeTime = other._dequeTime;
    }
    return *this;
}

PmergeMe::~PmergeMe()
{
}

int PmergeMe::parsePositiveInteger(const std::string &text)
{
    if (text.empty())
        throw std::runtime_error("Error");
    unsigned long value = 0;
    for (std::string::size_type i = 0; i < text.size(); ++i)
    {
        if (text[i] < '0' || text[i] > '9')
            throw std::runtime_error("Error");
        const unsigned int digit = text[i] - '0';
        if (value > (static_cast<unsigned long>(INT_MAX) - digit) / 10)
            throw std::runtime_error("Error");
        value = value * 10 + digit;
    }
    if (value == 0)
        throw std::runtime_error("Error");
    return static_cast<int>(value);
}

void PmergeMe::insertVectorPending(std::vector<VectorElement> &chain,
    const VectorPair &pending)
{
    std::vector<VectorElement>::size_type bound = chain.size();
    if (pending.highId != std::numeric_limits<unsigned long>::max())
    {
        for (std::vector<VectorElement>::size_type i = 0; i < chain.size(); ++i)
        {
            if (chain[i].id == pending.highId)
            {
                bound = i;
                break;
            }
        }
    }
    std::vector<VectorElement>::size_type left = 0;
    std::vector<VectorElement>::size_type right = bound;
    while (left < right)
    {
        const std::vector<VectorElement>::size_type middle = left
            + (right - left) / 2;
        if (chain[middle].value < pending.low.value)
            left = middle + 1;
        else
            right = middle;
    }
    chain.insert(chain.begin() + left, pending.low);
}

std::vector<PmergeMe::VectorElement> PmergeMe::fordJohnsonVector(
    const std::vector<VectorElement> &values)
{
    if (values.size() <= 1)
        return values;

    std::vector<VectorPair> pairs;
    std::vector<VectorElement> highs;
    for (std::vector<VectorElement>::size_type i = 0; i + 1 < values.size(); i += 2)
    {
        VectorElement low = values[i];
        VectorElement high = values[i + 1];
        if (low.value > high.value)
        {
            const VectorElement temporary = low;
            low = high;
            high = temporary;
        }
        pairs.push_back(VectorPair(low, high.id));
        highs.push_back(high);
    }
    const bool hasOdd = values.size() % 2 != 0;
    VectorElement odd;
    if (hasOdd)
        odd = values[values.size() - 1];

    highs = fordJohnsonVector(highs);
    std::vector<VectorElement> chain;
    std::vector<VectorPair> pending;
    for (std::vector<VectorElement>::size_type i = 0; i < highs.size(); ++i)
    {
        std::vector<VectorPair>::size_type pairIndex = 0;
        while (pairs[pairIndex].highId != highs[i].id)
            ++pairIndex;
        if (i == 0)
            chain.push_back(pairs[pairIndex].low);
        else
            pending.push_back(VectorPair(pairs[pairIndex].low, highs[i].id));
        chain.push_back(highs[i]);
    }
    if (hasOdd)
        pending.push_back(VectorPair(odd,
            std::numeric_limits<unsigned long>::max()));

    const std::vector<VectorPair>::size_type totalB = pending.size() + 1;
    std::vector<VectorPair>::size_type insertedThrough = 1;
    std::vector<VectorPair>::size_type previousJacob = 1;
    std::vector<VectorPair>::size_type currentJacob = 3;
    while (insertedThrough < totalB)
    {
        std::vector<VectorPair>::size_type upper = currentJacob;
        if (upper > totalB)
            upper = totalB;
        for (std::vector<VectorPair>::size_type b = upper;
             b > insertedThrough; --b)
            insertVectorPending(chain, pending[b - 2]);
        insertedThrough = upper;
        const std::vector<VectorPair>::size_type nextJacob = currentJacob
            + 2 * previousJacob;
        previousJacob = currentJacob;
        currentJacob = nextJacob;
    }
    return chain;
}

void PmergeMe::insertDequePending(std::deque<DequeElement> &chain,
    const DequePair &pending)
{
    std::deque<DequeElement>::size_type bound = chain.size();
    if (pending.highId != std::numeric_limits<unsigned long>::max())
    {
        for (std::deque<DequeElement>::size_type i = 0; i < chain.size(); ++i)
        {
            if (chain[i].id == pending.highId)
            {
                bound = i;
                break;
            }
        }
    }
    std::deque<DequeElement>::size_type left = 0;
    std::deque<DequeElement>::size_type right = bound;
    while (left < right)
    {
        const std::deque<DequeElement>::size_type middle = left
            + (right - left) / 2;
        if (chain[middle].value < pending.low.value)
            left = middle + 1;
        else
            right = middle;
    }
    chain.insert(chain.begin() + left, pending.low);
}

std::deque<PmergeMe::DequeElement> PmergeMe::fordJohnsonDeque(
    const std::deque<DequeElement> &values)
{
    if (values.size() <= 1)
        return values;

    std::deque<DequePair> pairs;
    std::deque<DequeElement> highs;
    for (std::deque<DequeElement>::size_type i = 0; i + 1 < values.size(); i += 2)
    {
        DequeElement low = values[i];
        DequeElement high = values[i + 1];
        if (low.value > high.value)
        {
            const DequeElement temporary = low;
            low = high;
            high = temporary;
        }
        pairs.push_back(DequePair(low, high.id));
        highs.push_back(high);
    }
    const bool hasOdd = values.size() % 2 != 0;
    DequeElement odd;
    if (hasOdd)
        odd = values[values.size() - 1];

    highs = fordJohnsonDeque(highs);
    std::deque<DequeElement> chain;
    std::deque<DequePair> pending;
    for (std::deque<DequeElement>::size_type i = 0; i < highs.size(); ++i)
    {
        std::deque<DequePair>::size_type pairIndex = 0;
        while (pairs[pairIndex].highId != highs[i].id)
            ++pairIndex;
        if (i == 0)
            chain.push_back(pairs[pairIndex].low);
        else
            pending.push_back(DequePair(pairs[pairIndex].low, highs[i].id));
        chain.push_back(highs[i]);
    }
    if (hasOdd)
        pending.push_back(DequePair(odd,
            std::numeric_limits<unsigned long>::max()));

    const std::deque<DequePair>::size_type totalB = pending.size() + 1;
    std::deque<DequePair>::size_type insertedThrough = 1;
    std::deque<DequePair>::size_type previousJacob = 1;
    std::deque<DequePair>::size_type currentJacob = 3;
    while (insertedThrough < totalB)
    {
        std::deque<DequePair>::size_type upper = currentJacob;
        if (upper > totalB)
            upper = totalB;
        for (std::deque<DequePair>::size_type b = upper;
             b > insertedThrough; --b)
            insertDequePending(chain, pending[b - 2]);
        insertedThrough = upper;
        const std::deque<DequePair>::size_type nextJacob = currentJacob
            + 2 * previousJacob;
        previousJacob = currentJacob;
        currentJacob = nextJacob;
    }
    return chain;
}

std::vector<int> PmergeMe::sortVector(const std::vector<int> &values)
{
    std::vector<VectorElement> tagged;
    for (std::vector<int>::size_type i = 0; i < values.size(); ++i)
        tagged.push_back(VectorElement(values[i], i));
    tagged = fordJohnsonVector(tagged);

    std::vector<int> result;
    for (std::vector<VectorElement>::size_type i = 0; i < tagged.size(); ++i)
        result.push_back(tagged[i].value);
    return result;
}

std::deque<int> PmergeMe::sortDeque(const std::deque<int> &values)
{
    std::deque<DequeElement> tagged;
    for (std::deque<int>::size_type i = 0; i < values.size(); ++i)
        tagged.push_back(DequeElement(values[i], i));
    tagged = fordJohnsonDeque(tagged);

    std::deque<int> result;
    for (std::deque<DequeElement>::size_type i = 0; i < tagged.size(); ++i)
        result.push_back(tagged[i].value);
    return result;
}

void PmergeMe::printSequence(const std::string &label,
    const std::vector<int> &values)
{
    std::cout << label;
    for (std::vector<int>::size_type i = 0; i < values.size(); ++i)
        std::cout << ' ' << values[i];
    std::cout << std::endl;
}

void PmergeMe::verifyResults() const
{
    if (_vectorResult.size() != _dequeResult.size())
        throw std::runtime_error("Error");
    for (std::vector<int>::size_type i = 0; i < _vectorResult.size(); ++i)
    {
        if (_vectorResult[i] != _dequeResult[i]
            || (i > 0 && _vectorResult[i - 1] > _vectorResult[i]))
            throw std::runtime_error("Error");
    }
}

void PmergeMe::run(int argc, char **argv)
{
    if (argc < 2)
        throw std::runtime_error("Error");
    _before.clear();
    _vectorResult.clear();
    _dequeResult.clear();

    const std::clock_t vectorStart = std::clock();
    for (int i = 1; i < argc; ++i)
        _before.push_back(parsePositiveInteger(argv[i]));
    _vectorResult = sortVector(_before);
    const std::clock_t vectorEnd = std::clock();

    const std::clock_t dequeStart = std::clock();
    std::deque<int> dequeInput;
    for (int i = 1; i < argc; ++i)
        dequeInput.push_back(parsePositiveInteger(argv[i]));
    _dequeResult = sortDeque(dequeInput);
    const std::clock_t dequeEnd = std::clock();

    _vectorTime = static_cast<double>(vectorEnd - vectorStart)
        * 1000000.0 / CLOCKS_PER_SEC;
    _dequeTime = static_cast<double>(dequeEnd - dequeStart)
        * 1000000.0 / CLOCKS_PER_SEC;
    verifyResults();

    printSequence("Before:", _before);
    printSequence("After:", _vectorResult);
    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _before.size()
              << " elements with std::vector : " << _vectorTime << " us"
              << std::endl;
    std::cout << "Time to process a range of " << _before.size()
              << " elements with std::deque  : " << _dequeTime << " us"
              << std::endl;
}
