#pragma once
#include <string>

class FilterOperation
{
    const std::string val;
public:
    FilterOperation(std::string);

    bool hasValue() const;
    std::string getValue() const;
};