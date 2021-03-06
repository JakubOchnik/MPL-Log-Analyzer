#pragma once 
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <string>
#include <variant>
#include <iostream>
#include <consts/consts.hpp>
#include <lib/utils.hpp>
#include <line_types/baseLine.hpp>

enum class ParseStage
{
    dateTime,
    src,
    type,
    thd,
    opt_id,
    opt_cli_comp,
    msg,
    structure
};


using pt = boost::posix_time::ptime;
using content_types = std::variant<std::string, pt, consts::LineType, long long>;
//using content_types = std::variant<std::string, LineType, long long>;

class LogLine: public BaseLine
{
    std::string lineType{"log"};

    std::string source;
    pt time;
    consts::LineType type{consts::LineType::debug};
    long long thd{};
    long long id{};
    std::string component;

    std::string message;
    std::string jsonStr;
    
    std::unordered_map<std::string, content_types> lineContents;

    void parseLine(std::string& rawLine);
    void saveToMap();

public:
    LogLine(std::string& rawLine);
    LogLine();

    std::variant<std::string, pt, consts::LineType, long long> getLineParameter(const std::string& name) const;

    [[nodiscard]] const std::string getFormattedOutput() const;
};