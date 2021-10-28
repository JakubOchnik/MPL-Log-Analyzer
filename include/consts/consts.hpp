#pragma once
#include <vector>
#include <array>
#include <map>
#include <string>

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
        contains,
        eq
    };

    enum class FilterOperationType
    {
        request,
        response
    };

    namespace condInputParams
    {
        const static std::string KEY = "-k";
        const static std::string EQUALS = "-e";
        const static std::string CONTAINS = "-c";
        const static std::string NOT_EQUALS = "-ne";
        const static std::string NOT_CONTAINS = "-nc";
    }

    const static std::map<std::string, CoreOperation> CoreOpMap = {
        {"filter", CoreOperation::filter},
        {"sort", CoreOperation::sort}
    };

    const static std::map<std::string, FilterOperationSpecifier> FilterOpSpecMap = {
        {"c", FilterOperationSpecifier::contains},
        {"e", FilterOperationSpecifier::eq}
    };

    const static std::map<std::string, FilterOperationType> FilterOpType = {
        {"request", FilterOperationType::request},
        {"response", FilterOperationType::response}
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
}
