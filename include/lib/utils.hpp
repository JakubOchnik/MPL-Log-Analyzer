#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>

class Utils
{
public:
    static bool isInternalLine(const std::string& line);
    static bool isInternalStart(const std::string& line);
    static void filterEscCode(std::string& phrase);

    static std::string int2hex(long long val, int width);
};