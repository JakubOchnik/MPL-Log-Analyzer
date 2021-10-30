#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <lib/utils.hpp>
#include <line_types/baseLine.hpp>

class InternalEntry: public BaseLine
{
    std::string lineType{"internal_err"};
    void parseError(std::string& rawLine, std::ifstream& file);

    std::string message;
public:
    InternalEntry(std::string& rawLine, std::ifstream& file);
};