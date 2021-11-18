#pragma once
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <consts/consts.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using pt = boost::posix_time::ptime;

struct RawOpStruct
{
    std::string coreOp{};
    bool neg{false};
    bool equalGl{false};
};

class Utils
{
public:
    static bool isInternalLine(const std::string& line);
    static bool isInternalStart(const std::string& line);
    static void filterEscCode(std::string& phrase);
    static pt string2pt(const std::string& source);

    static std::string int2hex(long long val, int width);

    static bool validateCoreOp(const std::string& op);
    static RawOpStruct parseOperation(const std::string& op);
};