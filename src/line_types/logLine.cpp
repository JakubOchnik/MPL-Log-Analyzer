#include <line_types/logLine.hpp>

LogLine::LogLine(std::string& rawLine)
{
    parseLine(rawLine);
}

LogLine::LogLine()
{
    lineContents = {
        {"time", time},
        {"source", source},
        {"type", type},
        {"thd", thd},
        {"id", id},
        {"component", component},
        {"message", message},
        {"jsonStr", jsonStr}
    };
}

using pt = boost::posix_time::ptime;

void LogLine::parseLine(std::string& rawLine)
{
    std::stringstream ss;
    std::size_t pos;
    // time
    pos = rawLine.find("|");
    std::string timeString = rawLine.substr(0,pos);
    timeString.replace(timeString.find("T"), 1, " ");
    try
    {
        time = boost::posix_time::time_from_string(timeString);
        lineContents["time"] = time;
    }
    catch(std::exception ex)
    {
        std::cout << ex.what();
    }
    rawLine = rawLine.substr(pos+1, rawLine.size());

    // source
    pos = rawLine.find("|");
    source = rawLine.substr(0,pos);
    boost::trim(source);
    lineContents["source"] = source;
    rawLine = rawLine.substr(pos+1, rawLine.size());

    // type
    pos = rawLine.find("|");
    std::string typeName = rawLine.substr(0,pos);
    Utils::filterEscCode(typeName);
    boost::trim(typeName);
    type = consts::LineTypeMap.at(typeName);
    lineContents["type"] = type;
    rawLine = rawLine.substr(pos+1, rawLine.size());

    // thd
    pos = rawLine.find("|");
    ss << std::hex << rawLine.substr(4, pos);
    ss >> thd;
    lineContents["thd"] = thd;
    rawLine = rawLine.substr(pos+1, rawLine.size());

    if(source == "mpl")
    {
        // opt_id
        pos = rawLine.find("|");
        ss << std::hex << rawLine.substr(0, pos);
        long long num;
        ss >> num;
        id = num;
        lineContents["id"] = id;
        rawLine = rawLine.substr(pos+1, rawLine.size());

        // component
        pos = rawLine.find("|");
        std::string compStr = rawLine.substr(0,pos);
        boost::trim(compStr);
        component = compStr;
        lineContents["component"] = component;
        rawLine = rawLine.substr(pos+1, rawLine.size());

        // message
        pos = rawLine.find("{");
        if(pos == std::string::npos)
        {
            message = rawLine;
            lineContents["message"] = message;
            return;
        }
        message = rawLine.substr(0, pos);
        lineContents["message"] = message;
        rawLine = rawLine.substr(pos, rawLine.size());
        // json
        jsonStr = rawLine;
        lineContents["jsonStr"] = jsonStr;
    }
    else
    {
        message = rawLine;
    }
}

std::variant<std::string, pt, consts::LineType, long long> LogLine::getLineParameter(const std::string& name)
{
    if(lineContents.find(name) == lineContents.end())
    {
        // TODO error handling
        return "error";
    }
    return lineContents[name];
}

const std::string LogLine::getFormattedOutput()
{
    
    std::string fTime{boost::posix_time::to_iso_extended_string(time)};
    std::string fType{consts::LineTypeInverseMap.at(type)};
    std::string fThd{Utils::int2hex(thd, 12)};
    std::string fId{Utils::int2hex(thd, 16)};
    std::string out = fTime + " | " \
    + source + " | " + fType + " |THD:" + fThd \
    + "|" + fId + "|" + component + "|" + message + jsonStr;
    return out;
}
