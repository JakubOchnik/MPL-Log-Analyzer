#include <funcFactory.hpp>

std::vector<std::function<bool(std::string, const LogLine&)>> FunctionFactory::createFiltersFromArgs(std::vector<KeyOperation> ops)
{
    std::vector<std::function<bool(std::string, const LogLine&)>> filters;
    for(auto& entry: ops)
    {
        switch(entry.op)
        {
            case consts::FilterOperationSpecifier::EQUALS:
                filters.push_back(getEqualFilterVisitor(entry.key, entry.negation));
                break;
            case consts::FilterOperationSpecifier::CONTAINS:
                filters.push_back(getContainsFilterVisitor(entry.key, entry.negation));
                break;
            case consts::FilterOperationSpecifier::LESS:
                filters.push_back(getInequalityFilter(entry.key, entry.negation, entry.equals, entry.op));
                break;
            case consts::FilterOperationSpecifier::GREATER:
                filters.push_back(getInequalityFilter(entry.key, entry.negation, entry.equals, entry.op));
                break;
        }
    }
    return filters;
}

std::function<bool(std::string, const LogLine&)> FunctionFactory::getEqualFilterVisitor(const std::string& key, const bool& neg)
{
    LogLine dummyBase;
    auto val = dummyBase.getLineParameter(key);
    // Type of our function depends on a type stored in variant
    // Dispatch, and then construct and return a fitting function

    // std::visit allows to execute a function on a currently stored type in variant
    // overload allows us to avoid declaring () operator overloads,
    // It can provide better readability, however I have to rethink whether this approach is really better in this case.

    return std::visit(overload{
        // a lambda, which captures a key, takes std::string as a param, and then constructs and returns another lambda...
        [key, neg](std::string&) {
            // out actual filter, which will be packed into std::function in order to put it in a vector
            return std::function<bool(std::string, const LogLine&)> {
                // captures a key, takes a string value and actually processed line as arguments 
                [key, neg](std::string value, const LogLine& line){
                    boost::to_lower(value);
                    // check if stored parameter is equal to value
                    if(!neg)
                        return std::get<std::string>(line.getLineParameter(key)) == value;
                    else
                        return !(std::get<std::string>(line.getLineParameter(key)) == value);
                }
            };
        },
        [key, neg](consts::LineType&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key, neg](std::string value, const LogLine& line){
                    boost::to_upper(value);
                    if(!neg)
                        return consts::LineTypeMap.at(value) == std::get<consts::LineType>(line.getLineParameter(key));
                    else
                        return !(consts::LineTypeMap.at(value) == std::get<consts::LineType>(line.getLineParameter(key)));
                }
            };
        },
        [key, neg](long long&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key, neg](std::string value, const LogLine& line){
                    if(!neg)
                        return std::get<long long>(line.getLineParameter(key)) == std::stoll(value);
                    else
                        return !(std::get<long long>(line.getLineParameter(key)) == std::stoll(value));
                }
            };
        },
        [key, neg](pt&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key, neg](std::string value, const LogLine& line){
                    if(!neg)
                        return std::get<pt>(line.getLineParameter(key)) == Utils::string2pt(value);
                    else
                        return !(std::get<pt>(line.getLineParameter(key)) == Utils::string2pt(value));
                }
            };
        },
    }, val);
};

std::function<bool(std::string, const LogLine&)> FunctionFactory::getContainsFilterVisitor(const std::string& key, const bool& neg)
{
    LogLine dummyBase;
    auto val = dummyBase.getLineParameter(key);
    return std::visit(overload{
        [key, neg](std::string&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key, neg](std::string value, const LogLine& line){
                    if(!neg)
                        return boost::algorithm::contains(std::get<std::string>(line.getLineParameter(key)), value);
                    else
                        return !(boost::algorithm::contains(std::get<std::string>(line.getLineParameter(key)), value));
                }
            };
        },
        [](consts::LineType&){return boolFunc{[](std::string, const LogLine&){return false;}};},
        [](long long&){return boolFunc{[](std::string, const LogLine&){return false;}};},
        [](pt&){return boolFunc{[](std::string, const LogLine&){return false;}};},
    }, val);
};

std::function<bool(std::string, const LogLine&)> FunctionFactory::getInequalityFilter(const std::string& key, const bool& neg, const bool& eq, consts::FilterOperationSpecifier op)
{
    LogLine dummyBase;
    auto val = dummyBase.getLineParameter(key);
    return std::visit(overload{
        [key, neg](std::string&){return boolFunc{[](std::string, const LogLine&){return false;}};},
        [](consts::LineType&){return boolFunc{[](std::string, const LogLine&){return false;}};},
        [key, neg, eq, op](long long&) {
            return op == consts::FilterOperationSpecifier::LESS ? boolFunc{
                [key, neg, eq](std::string value, const LogLine& line){
                    long long srcVal{std::get<long long>(line.getLineParameter(key))};
                    long long userVal{std::stoll(value)};
                    return (srcVal < userVal) || (eq && srcVal == userVal);
                }
            } : 
            boolFunc{
                [key, neg, eq](std::string value, const LogLine& line){
                    long long srcVal{std::get<long long>(line.getLineParameter(key))};
                    long long userVal{std::stoll(value)};
                    return (srcVal > userVal) || (eq && srcVal == userVal);
                }
            };
        },
        [key, neg, eq, op](pt&){
            return op == consts::FilterOperationSpecifier::LESS ? boolFunc{
                [key, neg, eq](std::string value, const LogLine& line){
                    pt srcVal{std::get<pt>(line.getLineParameter(key))};
                    size_t sepIndex{value.find("T")};
                    if(sepIndex != std::string::npos)
                    {
                        value.replace(sepIndex, 1, " ");
                    }
                    pt userVal{boost::posix_time::time_from_string(value)};
                    return (srcVal < userVal) || (eq && srcVal == userVal);
                }
            } : 
            boolFunc{
                [key, neg, eq](std::string value, const LogLine& line){
                    pt srcVal{std::get<pt>(line.getLineParameter(key))};
                    size_t sepIndex{value.find("T")};
                    if(sepIndex != std::string::npos)
                    {
                        value.replace(sepIndex, 1, " ");
                    }
                    pt userVal{boost::posix_time::time_from_string(value)};
                    return (srcVal > userVal) || (eq && srcVal == userVal);
                }
            };
        },
    }, val);
}