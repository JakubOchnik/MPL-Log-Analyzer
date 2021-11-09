#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using pt = boost::posix_time::ptime;

class Utils
{
public:
    static bool isInternalLine(const std::string& line);
    static bool isInternalStart(const std::string& line);
    static void filterEscCode(std::string& phrase);
    static pt string2pt(const std::string& source);

    static std::string int2hex(long long val, int width);
};