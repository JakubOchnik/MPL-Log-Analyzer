#include <vector>
#include <string>
#include <boost/algorithm/string.hpp>
#include <consts/consts.hpp>
#include <inputArgHandler.hpp>
#include <line_types/logLine.hpp>
#include <algorithm>
#include <functional>

// Create an object (a struct) which inherits from several lambdas ...
template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
// and then exposes their operator() for std::visit
template<class... Ts> overload(Ts...) -> overload<Ts...>; // Not needed in C++20

using boolFunc = std::function<bool(std::string, const LogLine&)>;

class FunctionFactory
{
public:
    static std::vector<std::function<bool(std::string, const LogLine&)>> createFiltersFromArgs(std::vector<KeyOperation> ops);

    static std::function<bool(std::string, const LogLine&)> getEqualFilterVisitor(const std::string& key);
    static std::function<bool(std::string, const LogLine&)> getContainsFilterVisitor(const std::string& key);
};
