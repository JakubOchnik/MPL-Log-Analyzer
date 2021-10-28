#include <operations/filter.hpp>

FilterOperation::FilterOperation(std::string value): val(value)
{
    
}

bool FilterOperation::hasValue() const
{
    return true;
}

std::string FilterOperation::getValue() const
{
    return val;
}