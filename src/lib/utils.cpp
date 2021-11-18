#include <lib/utils.hpp>

bool Utils::isInternalLine(const std::string& line)
{
    return(!(line[0] >= '0' && line[0] <= '9'));
}

bool Utils::isInternalStart(const std::string& line)
{
    return(boost::starts_with(line, "Traceback"));
}

void Utils::filterEscCode(std::string& phrase)
{
    if(boost::contains(phrase, "\033"))
    {
        size_t start = phrase.find("\033");
        size_t end = (phrase.substr(start, phrase.size())).find("m") + 1;
        while(start != std::string::npos && end != std::string::npos)
        {
            // \033 - m
            phrase.erase(start, end - start);
            start = phrase.find("\033");
            if(start == std::string::npos)
            {
                break;
            }
            end = start + (phrase.substr(start, phrase.size())).find("m") + 1;
        }
    }
}

std::string Utils::int2hex(long long val, int width)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(width) << std::hex << (val|0);
    return ss.str();
}

pt Utils::string2pt(const std::string& source)
{
    std::string srcCp = source;
    srcCp.replace(source.find("T"), 1, " ");
    return boost::posix_time::time_from_string(srcCp);
}

bool Utils::validateCoreOp(const std::string& op)
{
    using namespace consts;
    return std::find(allowedCoreOps.begin(), allowedCoreOps.end(), op) != allowedCoreOps.end();
}

RawOpStruct Utils::parseOperation(const std::string& op)
{
    if(op.empty() || op.size() == 1)
    {
        return RawOpStruct{};
    }
    // -e -c -ne -nc | -l -g -le -ge -nle -nge
    std::string parsedOp{op.substr(1)};
    if(parsedOp.size() == 1 && validateCoreOp(parsedOp))
    {
        // e c l g
        return RawOpStruct{parsedOp, false, false};
    }
    else if(parsedOp.size() == 2)
    {
        if(parsedOp[0] == 'n' && validateCoreOp(parsedOp.substr(1)))
        {
            // ne nc
            return RawOpStruct{parsedOp.substr(1), true, false};
        }
        else if(validateCoreOp(parsedOp.substr(0,1)))
        {
            // le ge
            return RawOpStruct{parsedOp.substr(0,1), false, true};
        }
    }
    else if(parsedOp.size() == 3)
    {
        // nle nge
        if(parsedOp[0] == 'n' && parsedOp[2] == 'e' && validateCoreOp(parsedOp.substr(1,1)))
        {
            return RawOpStruct{std::string{parsedOp[1]}, true, true};
        }
    }
    return RawOpStruct{};
}
