#include <funcFactory.hpp>

std::vector<std::function<bool(std::string, const LogLine&)>> FunctionFactory::createFiltersFromArgs(std::vector<KeyOperation> ops)
{
    std::vector<std::function<bool(std::string, const LogLine&)>> filters;
    for(auto& entry: ops)
    {
        if(entry.op == consts::FilterOperationSpecifier::eq)
        {
            filters.push_back(getEqualFilterVisitor(entry.key));
        }
        else
        {
            filters.push_back(getContainsFilterVisitor(entry.key));
        }
    }
    return filters;
}

std::function<bool(std::string, const LogLine&)> FunctionFactory::getEqualFilterVisitor(const std::string& key)
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
        [key](std::string&) {
            // out actual filter, which will be packed into std::function in order to put it in a vector
            return std::function<bool(std::string, const LogLine&)> {
                // captures a key, takes a string value and actually processed line as arguments 
                [key](std::string value, const LogLine& line){
                    boost::to_lower(value);
                    // check if stored parameter is equal to value
                    return std::get<std::string>(line.getLineParameter(key)) == value;
                }
            };
        },
        [key](consts::LineType&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key](std::string value, const LogLine& line){
                    boost::to_upper(value);
                    return consts::LineTypeMap.at(value) == std::get<consts::LineType>(line.getLineParameter(key));
                }
            };
        },
        [key](long long&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key](std::string value, const LogLine& line){
                    return std::get<long long>(line.getLineParameter(key)) == std::stoll(value);
                }
            };
        },
        [key](pt&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key](std::string value, const LogLine& line){
                    // NOT IMPLEMENTED TODO
                    return false;
                }
            };
        },
    }, val);
};

std::function<bool(std::string, const LogLine&)> FunctionFactory::getContainsFilterVisitor(const std::string& key)
{
    LogLine dummyBase;
    auto val = dummyBase.getLineParameter(key);
    return std::visit(overload{
        [key](std::string&) {
            return std::function<bool(std::string, const LogLine&)> {
                [key](std::string value, const LogLine& line){
                    return boost::algorithm::contains(std::get<std::string>(line.getLineParameter(key)), value);
                }
            };
        },
        [](consts::LineType&){return boolFunc{[](std::string, const LogLine&){return false;}};},
        [](long long&){return boolFunc{[](std::string, const LogLine&){return false;}};},
        [](pt&){return boolFunc{[](std::string, const LogLine&){return false;}};},
    }, val);
};