#pragma once
#include <string>

class BaseLine
{
    std::string lineType{"base"};
public:
    virtual std::string getLineType() const;
};