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
