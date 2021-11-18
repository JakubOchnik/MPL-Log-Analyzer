#pragma once
#include <vector>
#include <array>
#include <map>
#include <unordered_map>
#include <string>
#include <tuple>

namespace consts
{
    enum class LineType
    {
        debug,
        error,
        warn
    };

    const static std::map<const std::string, LineType> LineTypeMap =
    {
        {"DEBUG", LineType::debug},
        {"ERROR", LineType::error},
        {"WARN", LineType::warn}
    };

    const static std::map<const LineType, std::string> LineTypeInverseMap =
    {
        {LineType::debug, "DEBUG"},
        {LineType::error, "ERROR"},
        {LineType::warn, "WARN"}
    };

    enum class CoreOperation
    {
        filter,
        sort
    };

    enum class FilterOperationSpecifier
    {
        CONTAINS,
        EQUALS,
        LESS,
        GREATER
    };

    namespace condInputParams
    {
        const static std::string KEY = "-k";
        const static std::string EQUALS = "-e";
        const static std::string CONTAINS = "-c";
        const static std::string NOT_EQUALS = "-ne";
        const static std::string NOT_CONTAINS = "-nc";
    }

    const static std::unordered_map<std::string, FilterOperationSpecifier> PairParamMap = {
        {"e", FilterOperationSpecifier::EQUALS},
        {"c", FilterOperationSpecifier::CONTAINS},
        {"l", FilterOperationSpecifier::LESS},
        {"g", FilterOperationSpecifier::GREATER}
    };

    const static std::map<std::string, CoreOperation> CoreOpMap = {
        {"filter", CoreOperation::filter},
        {"sort", CoreOperation::sort}
    };

    const static std::map<std::string, FilterOperationSpecifier> FilterOpSpecMap = {
        {"c", FilterOperationSpecifier::CONTAINS},
        {"e", FilterOperationSpecifier::EQUALS}
    };

    const static std::vector<std::string> allowedCoreOps = {
        "e",
        "c",
        "l",
        "g"
    };

    enum class CondConcatMode
    {
        concat_or,
        concat_and
    };

    namespace startInputParams
    {
        const static std::string INPUT = "-i";
        const static std::string MODE_OR = "-or";
        const static std::string MODE_AND = "-and";
        const static std::string HELP = "-h";
    }

    namespace inequalityFilter
    {
        const static std::string LESS = "<";
        const static std::string MORE = ">";
    }
}

namespace textConsts
{
    const static std::string helpText = \
"   usage: mplLogAnalyzer [ initial arguments ] [ key ] [ condition ] ... \n\
    \n\
    Avaliable initial arguments:\n\
    -i      Input file\n\
    -and    Set condition concatenation mode to AND\n\
    -or     Set condition concatenation mode to OR\n\
    -h      Display help message\n\
    \n\
    Key-condition arguments:\n\
    -k      Key definition \n\
    -(n)e   Condition: (not)equals\n\
    -(n)c   Contition: (not)contains\n\
    -(n)g   Condition: (not)greater (NOT IMPLEMENTED)\n\
    -(n)l   Condition: (not)less (NOT IMPLEMENTED)\n\
    -(n)ge  Condition: (not)greater or equal (NOT IMPLEMENTED)\n\
    -(n)le  Condition: (not)less or equal (NOT IMPLEMENTED)\n\
    \n\
    Available keys:\n\
    -time, source, type, thd, id, component, message, jsonStr\n\
    \n\
    Example:\n\
    ./mplLogAnalyzer -i mpl.log -and -k source -e mpl -k message -nc fp-1\n";
}