#pragma once
#include <string>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <iomanip>

class Utils
{
public:
    static bool isInternalLine(const std::string& line);
    static bool isInternalStart(const std::string& line);
    static void filterEscCode(std::string& phrase);

    static int asInt();
    static std::string asString();
    static long long asLong();
    static std::string int2hex(long long val, int width);

    //getGen();
};